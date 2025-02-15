#include "game/ObjectFactory.h"
#include "game/objects/environment/Ground.h"
#include "game/objects/props/Tunnel.h"

std::shared_ptr<Object> ObjectFactory::Create(const ObjectConfig& config) {
    std::cout << "Creazione oggetto - Tipo: " << config.type
              << ", Subtype: " << config.subtype
              << ", ID: " << config.id << "\n";
    try {
        if(config.type == "Tunnel") {
            std::cout << "Posizione Tunnel: " << config.position[0] << ", "
                      << config.position[1] << ", " << config.position[2] << "\n";
            // Controllo parametri obbligatori
            if(config.subtype.empty()) {
                throw std::runtime_error("Tunnel senza subtype");
            }

            auto tunnel = std::make_shared<Tunnel>(Tunnel::TypeFromString(config.subtype));

            if (!tunnel->shader) {
                throw std::runtime_error("Shader 'texture' non caricato per Tunnel");
            }

            // Controllo dimensione position
            if(config.position.size() < 3) {
                throw std::runtime_error("Position non valida per Tunnel");
            }
            tunnel->pos = Vector3(config.position[0], config.position[1], config.position[2]);

            // Controllo dimensione scale
            if(config.scale.size() < 3) {
                throw std::runtime_error("Scale non valida per Tunnel");
            }
            tunnel->scale = Vector3(config.scale[0], config.scale[1], config.scale[2]);

            return tunnel;
        }
        else if(config.type == "Ground") {
            std::cout << "Posizione Ground: " << config.position[0] << ", "
                      << config.position[1] << ", " << config.position[2] << "\n";
            bool slope = (config.subtype == "SLOPE");
            auto ground = std::make_shared<Ground>(slope);

            if(config.scale.size() >= 3) {
                ground->scale = Vector3(config.scale[0], config.scale[1], config.scale[2]);
            }
            return ground;
        }
        // ... altri oggetti

    } catch(const std::exception& e) {
        std::cerr << "Errore creazione: " << e.what() << "\n";
    }
    return nullptr;
}