#include "core/engine/Engine.h"
#include "game/objects/base/Physical.h"
#include "game/DefaultScene.h"

#if defined(_WIN32)
#include <GL/wglew.h>
#else

#include <GL/glew.h>

#endif

#include <cmath>
#include <iostream>
#include <algorithm>
#include <memory>

Engine *GH_ENGINE = nullptr;
Player *GH_PLAYER = nullptr;
const Input *GH_INPUT = nullptr;
int GH_REC_LEVEL = 0;
int64_t GH_FRAME = 0;

Engine::Engine() {
	GH_ENGINE = this;
	GH_INPUT = &input;
	isFullscreen = false;

	isGood = InitOSWrapper();

	CreateGLWindow();
	InitGLObjects();
	SetupInputs();

	player = std::make_shared<Player>();
	GH_PLAYER = player.get();

	levelManager.RegisterLevel("level1", "assets/levels/level1.yaml");
	levelManager.RegisterLevel("level2", "assets/levels/level2.yaml");
	levelManager.RegisterLevel("level3", "assets/levels/level3.yaml");

	curScene = std::make_shared<DefaultScene>();
	LoadScene("level1");

	sky = std::make_shared<Sky>();
}

Engine::~Engine() {
	DestroyGLWindow();
}

int Engine::Run() {
	EnterMessageLoop();
	DestroyGLObjects();
	return 0;
}

void Engine::PeriodicRender(int64_t &cur_ticks) {
	//Used fixed time steps for updates
	const int64_t new_ticks = timer.GetTicks();
	for (int i = 0; cur_ticks < new_ticks && i < GH_MAX_STEPS; ++i) {
		Update();
		cur_ticks += ticks_per_step;
		GH_FRAME += 1;
		input.EndFrame();
	}
	cur_ticks = (cur_ticks < new_ticks ? new_ticks : cur_ticks);

	//Setup camera for rendering
	const float n = GH_CLAMP(NearestPortalDist() * 0.5f, GH_NEAR_MIN, GH_NEAR_MAX);
	main_cam.worldView = player->WorldToCam();
	main_cam.SetSize(iWidth, iHeight, n, GH_FAR);
	main_cam.UseViewport();

	//Render scene
	GH_REC_LEVEL = GH_MAX_RECURSION;
	Render(main_cam, 0, nullptr);
}

void Engine::LoadScene(const std::string &levelName) {
	if (!curScene) {
		std::cerr << "ERRORE: Scena non inizializzata\n";
		return;
	}
	try {
		auto config = levelManager.LoadConfig(levelName);
		std::cout << "Caricamento livello: " << config.name << "\n";
		std::cout << "Oggetti da caricare: " << config.objects.size() << "\n";

		if (config.objects.empty()) {
			std::cerr << "Attenzione: livello senza oggetti!\n";
		}

		// Pulizia oggetti esistenti
		vObjects.clear();
		vPortals.clear();
		player->Reset();

		// Carica gli oggetti dalla scena
		curScene->Load(config, vObjects, vPortals, *player);

		vObjects.push_back(player);

		std::cout << "Oggetti caricati: " << vObjects.size() << "\n";
		std::cout << "Portali caricati: " << vPortals.size() << "\n";
	} catch (const std::exception &e) {
		std::cerr << "Errore caricamento livello: " << e.what() << "\n";
	}

	// Fase di connessione avanzata
	for (auto &conn: pendingPortalConnections) {
		for (auto &targetPortal: vPortals) {
			if (targetPortal->sourceTunnel == conn.targetTunnel &&
			    targetPortal->doorNumber == conn.targetDoor) {
				Portal::Connect(conn.portal, targetPortal);
				break;
			}
		}
	}
	pendingPortalConnections.clear();
}

void Engine::Update() const {
	//Update
	for (const auto &vObject: vObjects) {
		assert(vObject.get());
		vObject->Update();
	}

	//Collisions
	//For each physics object
	for (size_t i = 0; i < vObjects.size(); ++i) {
		Physical *physical = vObjects[i]->AsPhysical();
		if (!physical) { continue; }
		Matrix4 worldToLocal = physical->WorldToLocal();

		//For each object to collide with
		for (size_t j = 0; j < vObjects.size(); ++j) {
			if (i == j) { continue; }
			Object &obj = *vObjects[j];
			if (!obj.mesh) { continue; }

			//For each hit sphere
			for (size_t s = 0; s < physical->hitSpheres.size(); ++s) {
				//Brings point from colliders local coordinates to hit's local coordinates.
				const Sphere &sphere = physical->hitSpheres[s];
				Matrix4 worldToUnit = sphere.LocalToUnit() * worldToLocal;
				Matrix4 localToUnit = worldToUnit * obj.LocalToWorld();
				Matrix4 unitToWorld = worldToUnit.Inverse();

				//For each collider
				for (size_t c = 0; c < obj.mesh->colliders.size(); ++c) {
					Vector3 push{};
					const Collider &collider = obj.mesh->colliders[c];
					if (collider.Collide(localToUnit, push)) {
						//If push is too small, just ignore
						push = unitToWorld.MulDirection(push);
						vObjects[j]->OnHit(*physical, push);
						physical->OnCollide(*vObjects[j], push);

						worldToLocal = physical->WorldToLocal();
						worldToUnit = sphere.LocalToUnit() * worldToLocal;
						localToUnit = worldToUnit * obj.LocalToWorld();
						unitToWorld = worldToUnit.Inverse();
					}
				}
			}
		}
	}

	//Portals
	for (const auto &vObject: vObjects) {
		Physical *physical = vObject->AsPhysical();
		if (physical) {
			for (const auto &vPortal: vPortals) {
				if (physical->TryPortal(*vPortal)) {
					break;
				}
			}
		}
	}
}

void Engine::Render(const Camera &cam, GLuint curFBO, const Portal *skipPortal) const {
	//Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT);
	sky->Draw(cam);

	//Create queries (if applicable)
	GLuint queries[GH_MAX_PORTALS];
	GLuint drawTest[GH_MAX_PORTALS];
	assert(vPortals.size() <= GH_MAX_PORTALS);
	if (occlusionCullingSupported) {
		glGenQueriesARB(static_cast<GLsizei>(vPortals.size()), queries);
	}

	//Draw scene
	for (const auto &vObject: vObjects) {
		vObject->Draw(cam, curFBO);
	}

	//Draw portals if possible
	if (GH_REC_LEVEL > 0) {
		//Draw portals
		GH_REC_LEVEL -= 1;
		if (occlusionCullingSupported && GH_REC_LEVEL > 0) {
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_FALSE);
			for (size_t i = 0; i < vPortals.size(); ++i) {
				if (vPortals[i].get() != skipPortal) {
					glBeginQueryARB(GL_SAMPLES_PASSED_ARB, queries[i]);
					vPortals[i]->DrawPink(cam);
					glEndQueryARB(GL_SAMPLES_PASSED_ARB);
				}
			}
			for (size_t i = 0; i < vPortals.size(); ++i) {
				if (vPortals[i].get() != skipPortal) {
					glGetQueryObjectuivARB(queries[i], GL_QUERY_RESULT_ARB, &drawTest[i]);
				}
			};
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);
			glDeleteQueriesARB(static_cast<GLsizei>(vPortals.size()), queries);
		}
		for (size_t i = 0; i < vPortals.size(); ++i) {
			if (vPortals[i].get() != skipPortal) {
				if (occlusionCullingSupported && (GH_REC_LEVEL > 0) && (drawTest[i] == 0)) {
					continue;
				} else {
					vPortals[i]->Draw(cam, curFBO);
				}
			}
		}
		GH_REC_LEVEL += 1;
	}

#if 0
	//Debug draw colliders
	for (size_t i = 0; i < vObjects.size(); ++i) {
	  vObjects[i]->DebugDraw(cam);
	}
#endif
}

void Engine::InitGLObjects() {
	// Debug info
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

	//Initialize extensions
	glewExperimental = GL_TRUE;
	glewInit();

	//Basic global variables
	glClearColor(0.6f, 0.9f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	//Check GL functionality
	glGetQueryiv(GL_SAMPLES_PASSED_ARB, GL_QUERY_COUNTER_BITS_ARB, &occlusionCullingSupported);

	EnableVSync();
}

void Engine::DestroyGLObjects() {
	curScene->Unload();
	vObjects.clear();
	vPortals.clear();
}

float Engine::NearestPortalDist() const {
	float dist = FLT_MAX;
	for (const auto &vPortal: vPortals) {
		dist = GH_MIN(dist, vPortal->DistTo(player->pos));
	}
	return dist;
}
