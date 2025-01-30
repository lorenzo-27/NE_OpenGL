#pragma once
#include "../base/Object.h"
#include "../../../resources/Resources.h"

class Statue : public Object {
public:
  Statue(const char* model) {
    mesh = AquireMesh(model);
    shader = AquireShader("texture");
    texture = AquireTexture("gold.bmp");
  }
  virtual ~Statue() {}
};
