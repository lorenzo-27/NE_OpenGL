#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>

class Shader {
public:
  explicit Shader(const char* name);
  ~Shader();

  void Use() const;
  void SetMVP(const float* mvp, const float* mv) const;

private:
  GLuint LoadShader(const char* fname, GLenum type);

  std::vector<std::string> attribs;
  GLuint vertId;
  GLuint fragId;
  GLuint progId;
  GLuint mvpId;
  GLuint mvId;
};
