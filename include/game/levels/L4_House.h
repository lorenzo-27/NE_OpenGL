#pragma once
#include "game/Scene.h"

class L4_House : public Scene {
public:
  L4_House(int rooms) : num_rooms(rooms) {}

  virtual void Load(PObjectVec& objs, PPortalVec& portals, Player& player) override;

private:
  int num_rooms;
};
