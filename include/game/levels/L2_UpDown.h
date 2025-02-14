#pragma once
#include "game/Scene.h"

class L2_UpDown : public Scene {
public:
  void Load(PObjectVec& objs, PPortalVec& portals, Player& player) override;
};
