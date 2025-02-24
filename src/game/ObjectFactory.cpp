#include "game/ObjectFactory.h"
#include "core/engine/Engine.h"
#include "game/objects/environment/Ground.h"
#include "game/objects/props/Tunnel.h"

std::shared_ptr<Object> ObjectFactory::Create(const ObjectConfig &config) {
	std::cout << "Creazione oggetto - Tipo: " << config.type
	          << ", Subtype: " << config.subtype
	          << ", ID: " << config.id << "\n";
	try {
		if (config.type == "Tunnel") {
			auto tunnel = std::make_shared<Tunnel>(Tunnel::TypeFromString(config.subtype));

			// Controllo dimensione position
			if (config.position.size() < 3) {
				throw std::runtime_error("Position non valida per Tunnel");
			}
			tunnel->pos = Vector3(config.position[0], config.position[1], config.position[2]);

			// Controllo dimensione scale
			if (config.scale.size() < 3) {
				throw std::runtime_error("Scale non valida per Tunnel");
			}
			tunnel->scale = Vector3(config.scale[0], config.scale[1], config.scale[2]);

			// Controllo dimensione rotation
			if (config.rotation.size() < 3) {
				throw std::runtime_error("Rotation non valida per Tunnel");
			}
			tunnel->euler = Vector3(config.rotation[0], config.rotation[1], config.rotation[2]);


			// Crea i portali per il tunnel
			std::vector<std::shared_ptr<Portal>> tunnelPortals;
			tunnel->CreatePortals(tunnelPortals, config.id);

			// Gestione connessioni
			for (const auto &portalConfig: config.portals) {
				int door = portalConfig["door"].as<int>();
				std::string target = portalConfig["connects_to"].as<std::string>();

				// Trova il portale corrispondente a questa porta
				for (auto &portal: tunnelPortals) {
					if (portal->doorNumber == door) {
						size_t dotPos = target.find('.');
						portal->connectedTunnel = target.substr(0, dotPos);
						portal->connectedDoor = std::stoi(target.substr(dotPos + 5)); // "doorX"

						// Aggiungi alla lista di connessioni pendenti
						GH_ENGINE->AddPortalConnection(portal, portal->connectedTunnel, portal->connectedDoor);
					}
				}
			}

			// Aggiungi i portali alla lista globale
			for (auto &portal: tunnelPortals) {
				GH_ENGINE->AddPortal(portal);
			}

			return tunnel;
		}
		if (config.type == "Ground") {
			bool slope = (config.subtype == "SLOPE");
			auto ground = std::make_shared<Ground>(slope);

			if (config.position.size() >= 3) {
				ground->pos = Vector3(config.position[0], config.position[1], config.position[2]);
			}
			if (config.scale.size() >= 3) {
				ground->scale = Vector3(config.scale[0], config.scale[1], config.scale[2]);
			}
			if (config.rotation.size() >= 3) {
				ground->euler = Vector3(config.rotation[0], config.rotation[1], config.rotation[2]);
			}
			return ground;
		}
	} catch (const std::exception &e) {
		std::cerr << "Errore creazione: " << e.what() << "\n";
	}
	return nullptr;
}
