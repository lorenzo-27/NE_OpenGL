#pragma once
#include "../base/Object.h"
#include "../../../resources/Resources.h"

class Statue : public Object {
public:
  Statue(const char* model) {
    mesh = AcquireMesh(model);
    shader = AcquireShader("texture");
    texture = AcquireTexture("gold.bmp");
  }

  ~Statue() override {}
};
