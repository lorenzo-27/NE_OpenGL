#pragma once
#include "../Scene.h"

class Level4 : public Scene {
public:
  void Load(PObjectVec& objs, PPortalVec& portals, Player& player) override;
};
