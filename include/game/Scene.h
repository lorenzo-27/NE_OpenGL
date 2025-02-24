#pragma once

#include "LevelConfig.h"
#include "game/objects/interactive/Player.h"

class Scene {
public:
	virtual ~Scene() = default;

	virtual void Load(const LevelConfig &config, PObjectVec &objs, PPortalVec &portals, Player &player) = 0;

	virtual void Unload() {}
};