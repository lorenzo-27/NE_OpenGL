#pragma once
#include <GL/glew.h>

class Texture {
public:
  Texture(const char* fname, int rows, int cols);
  ~Texture();

  void Use() const;

private:
  GLuint texId;
  bool is3D;
};
