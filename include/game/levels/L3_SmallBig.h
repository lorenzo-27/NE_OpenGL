#pragma once
#include "game/Scene.h"

class L3_SmallBig : public Scene {
public:
  virtual void Load(PObjectVec& objs, PPortalVec& portals, Player& player) override;
};
