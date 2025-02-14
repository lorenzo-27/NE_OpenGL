#pragma once
#include "game/Scene.h"

class L3_Scale : public Scene {
public:
  virtual void Load(PObjectVec& objs, PPortalVec& portals, Player& player) override;
};
