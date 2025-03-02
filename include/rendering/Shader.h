#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

class Shader {
public:
	explicit Shader(const char *name);

	~Shader();

	void Use() const;

	void SetMVP(const float *mvp, const float *mv) const;

	bool CheckForUpdates();

	bool LoadShaders();

private:

	static GLuint LoadGLSLShader(const char *fname, GLenum type);

	static GLuint LoadSPIRVShader(const char *fname, GLenum type);

	GLuint vertId;
	GLuint fragId;
	GLuint progId;
	GLuint mvpId;
	GLuint mvId;

	std::string name;
};
