#include "game/DefaultScene.h"

void DefaultScene::Load(const LevelConfig& config, PObjectVec& objs, PPortalVec& portals, Player& player) {
    Scene::Load(config, objs, portals, player);
}

void DefaultScene::Unload() {
    Scene::Unload();
}