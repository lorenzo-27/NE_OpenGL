#pragma once
#include "core/engine/GameHeader.h"
#include "game/objects/base/Object.h"
#include "rendering/FrameBuffer.h"
#include "rendering/Mesh.h"
#include "resources/Resources.h"
#include "rendering/Shader.h"
#include <memory>

class Portal : public Object {
public:
  //Subclass that represents a warp
  struct Warp {
    Warp(const Portal* fromPortal) : fromPortal(fromPortal), toPortal(nullptr) {
      delta.MakeIdentity();
      deltaInv.MakeIdentity();
    }

    Matrix4 delta;
    Matrix4 deltaInv;
    const Portal* fromPortal;
    const Portal* toPortal;
  };

  Portal();

  ~Portal() override {}

  virtual void Draw(const Camera& cam, GLuint curFBO) override;
  void DrawPink(const Camera& cam) const;

  Vector3 GetBump(const Vector3& a) const;
  const Warp* Intersects(const Vector3& a, const Vector3& b, const Vector3& bump) const;
  float DistTo(const Vector3& pt) const;

  static void Connect(const std::shared_ptr<Portal>& a, const std::shared_ptr<Portal>& b);
  static void Connect(Warp& a, Warp& b);

  Warp front;
  Warp back;

private:
  std::shared_ptr<Shader> errShader;
  FrameBuffer frameBuf[GH_MAX_RECURSION <= 1 ? 1 : GH_MAX_RECURSION - 1];
};
typedef std::vector<std::shared_ptr<Portal>> PPortalVec;