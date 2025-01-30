#pragma once
#include "objects/base/Object.h"
#include "objects/interactive/Portal.h"
#include "objects/interactive/Player.h"

class Scene {
public:
  virtual void Load(PObjectVec& objs, PPortalVec& portals, Player& player)=0;
  virtual void Unload() {};
};
