#pragma once
#include "game/objects/base/Object.h"
#include "resources/Resources.h"

class PillarRoom : public Object {
public:
  PillarRoom() {
    mesh = AcquireMesh("pillar_room.obj");
    shader = AcquireShader("texture");
    texture = AcquireTexture("three_room.bmp");
    scale = Vector3(1.1f);
  }

  void SetPortal(Object& portal) const {
    portal.pos = LocalToWorld().MulPoint(Vector3(0, 1.5f, -1));
    portal.euler = euler;
    portal.euler.y -= GH_PI / 2;
    portal.scale = Vector3(1, 1.5f, 1) * scale;
  }
  ~PillarRoom() override {}
};