#pragma once
#include "game/objects/base/Object.h"
#include "game/objects/interactive/Portal.h"
#include "game/objects/interactive/Player.h"

class Scene {
public:
  virtual ~Scene() = default;

  virtual void Load(PObjectVec& objs, PPortalVec& portals, Player& player)=0;
  virtual void Unload() {};
};
