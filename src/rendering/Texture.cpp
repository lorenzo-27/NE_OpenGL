#include "rendering/Texture.h"
#include <fstream>
#include <cassert>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char *fname, int rows, int cols, TextureType type) : type(type) {
	std::string path = std::string("assets/textures/") + fname;
	std::string ext = path.substr(path.find_last_of('.') + 1);

	// Check file extension to determine loading method
	if (ext == "bmp") {
		LoadBMP(path, rows, cols);
	} else if (ext == "hdr") {
		isHDR = true;
		LoadHDR(path);
	} else {
		// Handle other formats with stb_image
		LoadSTB(path);
	}
}

bool Texture::LoadBMP(const std::string& path, int rows, int cols) {
	// Check if this is a 3D texture
	assert(rows >= 1 && cols >= 1);
	is3D = (rows > 1 || cols > 1);

	// Open the bitmap
	std::ifstream fin(path, std::ios::in | std::ios::binary);
	if (!fin) {
		std::cerr << "Failed to open texture file: " << path << std::endl;
		texId = 0;
		return false;
	}

	// Read the bitmap
	char input[54];
	fin.read(input, 54);
	const GLsizei width = *reinterpret_cast<int32_t *>(&input[18]);
	const GLsizei height = *reinterpret_cast<int32_t *>(&input[22]);
	assert(width % cols == 0);
	assert(height % rows == 0);
	const int block_w = width / cols;
	const int block_h = height / rows;
	auto *img = new uint8_t[width * height * 3];
	for (int y = height; y-- > 0;) {
		const int row = y / block_h;
		const int ty = y % block_h;
		for (int x = 0; x < width; x++) {
			const int col = x / block_w;
			const int tx = x % block_w;
			uint8_t *ptr = img + ((row * cols + col) * (block_w * block_h) + ty * block_w + tx) * 3;
			fin.read(reinterpret_cast<char *>(ptr), 3);
		}
		const int padding = (width * 3) % 4;
		if (padding) {
			char junk[3];
			fin.read(junk, 4 - padding);
		}
	}

	// Load texture into video memory
	glGenTextures(1, &texId);
	if (is3D) {
		glBindTexture(GL_TEXTURE_2D_ARRAY, texId);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_GENERATE_MIPMAP, GL_TRUE);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB8, width / rows, height / cols, rows * cols, 0, GL_BGR,
		             GL_UNSIGNED_BYTE, img);
	} else {
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, img);
	}

	// Cleanup
	delete[] img;
	return true;
}

bool Texture::LoadHDR(const std::string& path) {
	int width, height, channels;
	float* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
	if (!data) {
		std::cerr << "Failed to load HDR texture: " << path << std::endl;
		texId = 0;
		return false;
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	// Use floating point format for HDR
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	return true;
}

bool Texture::LoadSTB(const std::string& path) {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data) {
		std::cerr << "Failed to load texture: " << path << std::endl;
		texId = 0;
		return false;
	}

	GLenum format;
	GLenum internalFormat;
	if (channels == 1) {
		format = GL_RED;
		internalFormat = GL_R8;
	} else if (channels == 3) {
		format = GL_RGB;
		internalFormat = GL_RGB8;
	} else if (channels == 4) {
		format = GL_RGBA;
		internalFormat = GL_RGBA8;
	} else {
		std::cerr << "Unsupported number of channels: " << channels << std::endl;
		stbi_image_free(data);
		texId = 0;
		return false;
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	// Choose appropriate format based on texture type
	if (type == TextureType::NORMAL || type == TextureType::HEIGHT) {
		// Normal maps need more precision
		internalFormat = GL_RGB16F;
	} else if (type == TextureType::ROUGHNESS || type == TextureType::METALLIC || type == TextureType::AO) {
		// These PBR maps are often grayscale
		internalFormat = (channels == 1) ? GL_R8 : GL_RGB8;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	return true;
}

Texture::~Texture() {
	glDeleteTextures(1, &texId);
}

void Texture::Use(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	if (is3D) {
		glBindTexture(GL_TEXTURE_2D_ARRAY, texId);
	} else {
		glBindTexture(GL_TEXTURE_2D, texId);
	}
}