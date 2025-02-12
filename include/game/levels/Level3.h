#pragma once
#include "game/Scene.h"

class Level3 : public Scene {
public:
  virtual void Load(PObjectVec& objs, PPortalVec& portals, Player& player) override;
};
