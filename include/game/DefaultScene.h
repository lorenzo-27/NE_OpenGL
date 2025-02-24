#pragma once

#include "Scene.h"

class DefaultScene : public Scene {
public:
	void Load(const LevelConfig &config, PObjectVec &objs, PPortalVec &portals, Player &player) override;

	void Unload() override;
};