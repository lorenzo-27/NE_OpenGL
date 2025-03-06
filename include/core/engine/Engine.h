#pragma once

#include "GameHeader.h"
#include "core/camera/Camera.h"
#include "core/input/Input.h"
#include "game/objects/base/Object.h"
#include "game/objects/interactive/Portal.h"
#include "game/objects/interactive/Player.h"
#include "Timer.h"
#include "game/Scene.h"
#include "game/objects/environment/Sky.h"
#include "game/LevelManager.h"
#include <GL/glew.h>

#if defined(_WIN32)
#include <windows.h>
#else

#include <SDL2/SDL.h>

#endif

#include <memory>
#include <vector>

class InputAdapter;

class Engine {
public:
	Engine();

	~Engine();

	int Run();

	void Update() const;

	void Render(const Camera &cam, GLuint curFBO, const Portal *skipPortal) const;

	void LoadScene(const std::string &levelName);

	void AddPortal(const std::shared_ptr<Portal> &portal) {
		vPortals.push_back(portal);
	}

	void AddPortalConnection(const std::shared_ptr<Portal> &portal, const std::string &targetTunnel, int targetDoor) {
		pendingPortalConnections.push_back({portal, targetTunnel, targetDoor});
	}

#if defined(_WIN32)
	LRESULT WindowProc(HWND hCurWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

	[[nodiscard]] const Player &GetPlayer() const { return *player; }

	[[nodiscard]] float NearestPortalDist() const;


private:
	static bool InitOSWrapper();

	void CreateGLWindow();

	void DestroyGLWindow();

	void InitGLObjects();

	void DestroyGLObjects();

	void SetupInputs();

	void ConfineCursor();

	void ToggleFullscreen();

	int EnterMessageLoop();

	void PeriodicRender(int64_t &cur_ticks);

	static void EnableVSync();

#if defined(_WIN32)
	HWND  hWnd = nullptr;         // window
	HDC   hDC = nullptr;          // device context
	HGLRC hRC = nullptr;          // opengl context
	HINSTANCE hInstance;          // process id

	LONG iWidth;         // window width
	LONG iHeight;        // window height
#else
	SDL_Window *window = nullptr;
	SDL_GLContext glContext = nullptr;
	int iWidth = 0;
	int iHeight = 0;
#endif

	int64_t ticks_per_step = 0;

    bool isGood = false; // initialized without problems
    bool isWindowGood = false; // window successfully created and initialized
    bool isFullscreen; // fullscreen state

	Camera main_cam;
	Input input;
	Timer timer;

	struct PortalConnection {
		std::shared_ptr<Portal> portal;
		std::string targetTunnel;
		int targetDoor;
	};

	std::vector<PortalConnection> pendingPortalConnections;
	std::vector<std::shared_ptr<Object> > vObjects;
	std::vector<std::shared_ptr<Portal> > vPortals;
	std::shared_ptr<Sky> sky;
	std::shared_ptr<Player> player;

	GLint occlusionCullingSupported{};

	LevelManager levelManager;
	std::shared_ptr<Scene> curScene = nullptr;
	std::unique_ptr<InputAdapter> inputAdapter;
};
