#pragma once
#include "game/objects/base/Object.h"
#include "resources/Resources.h"

class Pillar : public Object {
public:
  Pillar() {
    mesh = AcquireMesh("pillar.obj");
    shader = AcquireShader("texture");
    texture = AcquireTexture("white.bmp");
    scale = Vector3(0.1f);
  }

  ~Pillar() override {}
};
