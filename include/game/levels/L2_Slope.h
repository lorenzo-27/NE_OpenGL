#pragma once
#include "game/Scene.h"

class L2_Slope : public Scene {
public:
  void Load(PObjectVec& objs, PPortalVec& portals, Player& player) override;
};
