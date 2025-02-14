#pragma once
#include "game/Scene.h"

class L1_DoubleTunnel : public Scene {
public:
  virtual void Load(PObjectVec& objs, PPortalVec& portals, Player& player) override;
};
