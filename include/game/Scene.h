#pragma once
#include "objects/base/Object.h"
#include "objects/interactive/Portal.h"
#include "objects/interactive/Player.h"

class Scene {
public:
  virtual ~Scene() = default;

  virtual void Load(PObjectVec& objs, PPortalVec& portals, Player& player)=0;
  virtual void Unload() {};
};
