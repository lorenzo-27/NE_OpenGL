#pragma once

#include <GL/glew.h>
#include <string>

enum class TextureType {
	DIFFUSE,
	NORMAL,
	ROUGHNESS,
	METALLIC,
	AO,
	HEIGHT,
	HDR
};

class Texture {
public:
	Texture(const char *fname, int rows = 1, int cols = 1, TextureType type = TextureType::DIFFUSE);
	~Texture();

	void Use(unsigned int slot = 0) const;

	bool IsHDR() const { return isHDR; }
	bool Is3D() const { return is3D; }
	TextureType GetType() const { return type; }
	GLuint GetID() const { return texId; }

private:
	bool LoadBMP(const std::string& path, int rows, int cols);
	bool LoadHDR(const std::string& path);
	bool LoadSTB(const std::string& path);

	GLuint texId{0};
	bool is3D{false};
	bool isHDR{false};
	TextureType type;
};