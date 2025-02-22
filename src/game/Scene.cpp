#include "game/Scene.h"
#include "game/ObjectFactory.h"

void Scene::Load(const LevelConfig& config, PObjectVec& objs, PPortalVec& portals, Player& player) {
    // Controllo player start
    if(config.player_start.size() < 3) {
        throw std::runtime_error("Player start non valido nel livello");
    }
    player.SetPosition(Vector3(
        config.player_start[0],
        config.player_start[1],
        config.player_start[2]
    ));

    std::cout << "Inizio caricamento scena\n";

    for(const auto& objConfig : config.objects) {
        std::cout << "Processando oggetto: " << objConfig.type << "\n";
        if(auto obj = ObjectFactory::Create(objConfig)) {
            std::cout << "Oggetto creato con successo: " << objConfig.type
                      << " a posizione (" << obj->pos.x << ", "
                      << obj->pos.y << ", " << obj->pos.z << ")\n";
            objs.push_back(obj);
        }
        else {
            std::cerr << "Fallita creazione oggetto: " << objConfig.type << "\n";
        }
    }
    std::cout << "Caricamento completato. Oggetti totali: " << objs.size() << "\n";
}