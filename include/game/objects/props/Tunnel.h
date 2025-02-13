#pragma once
#include "game/objects/base/Object.h"

class Tunnel : public Object {
public:
  enum Type {
    NORMAL = 0,
    SCALE = 1,
    SLOPE = 2,
  };

  Tunnel(Type type) : type(type) {
    if (type == SCALE) {
      mesh = AcquireMesh("tunnel_scale.obj");
    } else if (type == SLOPE) {
      mesh = AcquireMesh("tunnel_slope.obj");
    } else {
      mesh = AcquireMesh("tunnel.obj");
    }
    shader = AcquireShader("texture");
    texture = AcquireTexture("tunnel.bmp");
  }
  virtual ~Tunnel() {}

  void SetDoor1(Object& portal) const {
    portal.pos = LocalToWorld().MulPoint(Vector3(0, 1, 1));
    portal.euler = euler;
    portal.scale = Vector3(0.6f, 0.999f, 1) * scale.x;
  }
  void SetDoor2(Object& portal) const {
    portal.euler = euler;
    if (type == SCALE) {
      portal.pos = LocalToWorld().MulPoint(Vector3(0, 0.5f, -1));
      portal.scale = Vector3(0.3f, 0.499f, 0.5f) * scale.x;
    } else if (type == SLOPE) {
      portal.pos = LocalToWorld().MulPoint(Vector3(0, -1, -1));
      portal.scale = Vector3(0.6f, 0.999f, 1) * scale.x;
    } else {
      portal.pos = LocalToWorld().MulPoint(Vector3(0, 1, -1));
      portal.scale = Vector3(0.6f, 0.999f, 1) * scale.x;
    }
  }

private:
  Type type;
};
