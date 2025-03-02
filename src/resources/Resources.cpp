#include "resources/Resources.h"
#include <unordered_map>
#include <iostream>

// Global repository of loaded resources
static std::unordered_map<std::string, std::weak_ptr<Mesh>> meshMap;
static std::unordered_map<std::string, std::weak_ptr<Shader>> shaderMap;
static std::unordered_map<std::string, std::weak_ptr<Texture>> textureMap;

std::shared_ptr<Mesh> AcquireMesh(const char *name) {
	std::weak_ptr<Mesh> &mesh = meshMap[std::string(name)];
	if (mesh.expired()) {
		std::shared_ptr<Mesh> newMesh(new Mesh(name));
		mesh = newMesh;
		return newMesh;
	} else {
		return mesh.lock();
	}
}

std::shared_ptr<Shader> AcquireShader(const char *name) {
	std::weak_ptr<Shader> &shader = shaderMap[std::string(name)];
	if (shader.expired()) {
		std::shared_ptr<Shader> newShader(new Shader(name));
		shader = newShader;
		return newShader;
	} else {
		return shader.lock();
	}
}

std::shared_ptr<Texture> AcquireTexture(const char *name, int rows, int cols, TextureType type) {
	// Create a unique key that includes texture type
	std::string key = std::string(name) + "_" + std::to_string(static_cast<int>(type));

	std::weak_ptr<Texture> &tex = textureMap[key];
	if (tex.expired()) {
		std::shared_ptr<Texture> newTex(new Texture(name, rows, cols, type));
		tex = newTex;
		return newTex;
	} else {
		return tex.lock();
	}
}

// Function to check for shader updates
void CheckForShaderUpdates(bool forceReload) {
	// Unbind any current shader before reloading
	glUseProgram(0);
	glFlush();
	glFinish();

	for (auto& pair : shaderMap) {
		if (!pair.second.expired()) {
			std::shared_ptr<Shader> shader = pair.second.lock();
			if (forceReload) {
				// Force reload the shader
				bool success = shader->LoadShaders();
				std::cout << "Shader " << pair.first << " "
				          << (success ? "force reloaded successfully" : "reload failed") << std::endl;
			} else {
				// Check for updates normally
				shader->CheckForUpdates();
			}
		}
	}

	// Final flush to ensure all shader operations complete
	glFlush();
	glFinish();
}

// Function to reload a specific shader by name
bool ReloadShader(const char* name) {
	auto it = shaderMap.find(std::string(name));
	if (it != shaderMap.end() && !it->second.expired()) {
		std::cout << "Forcibly recreating shader: " << name << std::endl;

		// Unbind any current shader
		glUseProgram(0);
		glFlush();
		glFinish();

		// Create a completely new shader object
		std::shared_ptr<Shader> newShader(new Shader(name));
		it->second = newShader;

		return true;
	}
	return false;
}