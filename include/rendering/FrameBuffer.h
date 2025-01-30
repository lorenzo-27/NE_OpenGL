#pragma once
#include "../core/camera/Camera.h"
#include <GL/glew.h>

//Forward declaration
class Portal;

class FrameBuffer {
public:
  FrameBuffer();

  ~FrameBuffer();

  void Render(const Camera& cam, GLuint curFBO, const Portal* skipPortal) const;
  void Use() const;

private:
  GLuint texId;
  GLuint fbo;
  GLuint renderBuf;
};
