#pragma once
#include "game/objects/base/Object.h"
#include "resources/Resources.h"

class Ground final : public Object {
public:
  explicit Ground(bool slope=false) {
    if (slope) {
      mesh = AcquireMesh("ground_slope.obj");
    } else {
      mesh = AcquireMesh("ground.obj");
    }
    shader = AcquireShader("texture");
    texture = AcquireTexture("floor.bmp");
    scale = Vector3(10, 1, 10);
  }

  ~Ground() override {}
};
