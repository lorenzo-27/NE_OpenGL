#pragma once

#include <GL/glew.h>
#include <string>

class Shader {
public:
	explicit Shader(const char *name);

	~Shader();

	void Use() const;

	void SetMVP(const float *mvp, const float *mv) const;

private:
	static GLuint LoadShader(const char *fname, GLenum type);

	GLuint vertId;
	GLuint fragId;
	GLuint progId;
	GLuint mvpId;
	GLuint mvId;
};
