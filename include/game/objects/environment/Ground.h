#pragma once
#include "../base/Object.h"
#include "../../../resources/Resources.h"

class Ground : public Object {
public:
  Ground(bool slope=false) {
    if (slope) {
      mesh = AcquireMesh("ground_slope.obj");
    } else {
      mesh = AcquireMesh("ground.obj");
    }
    shader = AcquireShader("texture");
    texture = AcquireTexture("checker_green.bmp");
    scale = Vector3(10, 1, 10);
  }

  ~Ground() override {}
};
