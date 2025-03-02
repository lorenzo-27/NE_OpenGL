#include "rendering/Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <chrono>

// File timestamp cache for hot-reloading
struct ShaderFileInfo {
	std::filesystem::file_time_type lastModified;
	std::string path;
};

static std::unordered_map<std::string, ShaderFileInfo> vertexShaderFiles;
static std::unordered_map<std::string, ShaderFileInfo> fragmentShaderFiles;

Shader::Shader(const char *name) : vertId(0), fragId(0), progId(0),
                                   mvpId(-1), mvId(-1), name(name) {
	LoadShaders();
}

Shader::~Shader() {
	glDeleteProgram(progId);
	glDeleteShader(vertId);
	glDeleteShader(fragId);
}

bool Shader::LoadShaders() {
	// Delete old shader program and objects
	if (progId) {
		// Make sure the shader is not in use before deletion
		glUseProgram(0);

		// Delete existing objects
		glDeleteProgram(progId);
		glDeleteShader(vertId);
		glDeleteShader(fragId);

		// Reset IDs
		progId = 0;
		vertId = 0;
		fragId = 0;

		// Reset uniform locations
		mvpId = -1;
		mvId = -1;
	}

	// Force GPU pipeline flush
	glFlush();
	glFinish();

	// Get file paths
	std::string vertPath = "assets/shaders/" + std::string(name) + ".vert";
	std::string fragPath = "assets/shaders/" + std::string(name) + ".frag";
	std::string spirvVertPath = "assets/shaders/" + std::string(name) + ".vert.spv";
	std::string spirvFragPath = "assets/shaders/" + std::string(name) + ".frag.spv";

	std::cout << "Checking shader files for: " << name << std::endl;

	// Store file info for hot-reloading
	try {
		if (std::filesystem::exists(vertPath)) {
			vertexShaderFiles[name] = {std::filesystem::last_write_time(vertPath), vertPath};
			std::cout << "Found vertex shader: " << vertPath << std::endl;
		} else if (std::filesystem::exists(spirvVertPath)) {
			vertexShaderFiles[name] = {std::filesystem::last_write_time(spirvVertPath), spirvVertPath};
			std::cout << "Found SPIR-V vertex shader: " << spirvVertPath << std::endl;
		}

		if (std::filesystem::exists(fragPath)) {
			fragmentShaderFiles[name] = {std::filesystem::last_write_time(fragPath), fragPath};
			std::cout << "Found fragment shader: " << fragPath << std::endl;
		} else if (std::filesystem::exists(spirvFragPath)) {
			// FIX: usa spirvFragPath invece di fragPath
			fragmentShaderFiles[name] = {std::filesystem::last_write_time(spirvFragPath), spirvFragPath};
			std::cout << "Found SPIR-V fragment shader: " << spirvFragPath << std::endl;
		}
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << "\n";
	}

	// Try SPIR-V first, then fallback to GLSL
	bool useSpirV = std::filesystem::exists(spirvVertPath) && std::filesystem::exists(spirvFragPath);

	if (useSpirV) {
		vertId = LoadSPIRVShader(spirvVertPath.c_str(), GL_VERTEX_SHADER);
		fragId = LoadSPIRVShader(spirvFragPath.c_str(), GL_FRAGMENT_SHADER);
	} else {
		vertId = LoadGLSLShader(vertPath.c_str(), GL_VERTEX_SHADER);
		fragId = LoadGLSLShader(fragPath.c_str(), GL_FRAGMENT_SHADER);
	}

	if (!vertId || !fragId) {
		std::cerr << "Failed to load shaders: " << name << "\n";
		return false;
	}

	// Create program
	progId = glCreateProgram();
	glAttachShader(progId, vertId);
	glAttachShader(progId, fragId);

	// Link program
	glLinkProgram(progId);

	// Check linking errors
	GLint success;
	glGetProgramiv(progId, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(progId, 512, nullptr, infoLog);
		std::cerr << "Shader linking failed (" << name << "):\n" << infoLog << "\n";
		return false;
	}

	// Get uniform locations
	mvpId = glGetUniformLocation(progId, "mvp");
	mvId = glGetUniformLocation(progId, "mv");

	std::cout << "Shader " << name << " " << (useSpirV ? "[SPIR-V]" : "[GLSL]") << " loaded successfully.\n";

	// After successful loading, force another flush
	glFlush();
	glFinish();

	return true;
}

bool Shader::CheckForUpdates() {
	bool updated = false;

	// Always recheck the files directly instead of relying only on cached info
	std::string vertPath = "assets/shaders/" + std::string(name) + ".vert";
	std::string fragPath = "assets/shaders/" + std::string(name) + ".frag";
	std::string spirvVertPath = "assets/shaders/" + std::string(name) + ".vert.spv";
	std::string spirvFragPath = "assets/shaders/" + std::string(name) + ".frag.spv";

	try {
		// Check all possible shader files for the vertex shader
		if (std::filesystem::exists(vertPath)) {
			auto lastMod = std::filesystem::last_write_time(vertPath);
			auto it = vertexShaderFiles.find(name);
			if (it != vertexShaderFiles.end() && lastMod > it->second.lastModified) {
				std::cout << "Vertex shader modified: " << vertPath << std::endl;
				updated = true;
				vertexShaderFiles[name].lastModified = lastMod;
			}
		}

		if (std::filesystem::exists(spirvVertPath)) {
			auto lastMod = std::filesystem::last_write_time(spirvVertPath);
			auto it = vertexShaderFiles.find(name);
			if (it != vertexShaderFiles.end() && lastMod > it->second.lastModified) {
				std::cout << "SPIR-V vertex shader modified: " << spirvVertPath << std::endl;
				updated = true;
				vertexShaderFiles[name].lastModified = lastMod;
			}
		}

		// Check all possible shader files for the fragment shader
		if (std::filesystem::exists(fragPath)) {
			auto lastMod = std::filesystem::last_write_time(fragPath);
			auto it = fragmentShaderFiles.find(name);
			if (it != fragmentShaderFiles.end() && lastMod > it->second.lastModified) {
				std::cout << "Fragment shader modified: " << fragPath << std::endl;
				updated = true;
				fragmentShaderFiles[name].lastModified = lastMod;
			}
		}

		if (std::filesystem::exists(spirvFragPath)) {
			auto lastMod = std::filesystem::last_write_time(spirvFragPath);
			auto it = fragmentShaderFiles.find(name);
			if (it != fragmentShaderFiles.end() && lastMod > it->second.lastModified) {
				std::cout << "SPIR-V fragment shader modified: " << spirvFragPath << std::endl;
				updated = true;
				fragmentShaderFiles[name].lastModified = lastMod;
			}
		}
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Filesystem error during shader update check: " << e.what() << "\n";
	}

	// Reload if necessary
	if (updated) {
		std::cout << "Shader " << name << " changed, reloading...\n";
		return LoadShaders();
	}

	return false;
}

GLuint Shader::LoadGLSLShader(const char *fname, GLenum type) {
	std::ifstream file(fname);
	if (!file.is_open()) {
		std::cerr << "Failed to open shader file: " << fname << "\n";
		return 0;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();

	// Add a timestamp comment to invalidate driver cache
	auto now = std::chrono::system_clock::now();
	auto time_t_now = std::chrono::system_clock::to_time_t(now);
	std::stringstream timestamp;
	timestamp << "\n// Shader reload timestamp: " << time_t_now << "\n";

	// Find the first line (usually version directive) and insert the timestamp after it
	size_t pos = content.find('\n');
	if (pos != std::string::npos) {
		content.insert(pos + 1, timestamp.str());
	} else {
		content = timestamp.str() + content;
	}

	const char *source = content.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader compilation failed (" << fname << "):\n" << infoLog << "\n";
		return 0;
	}

	return shader;
}

GLuint Shader::LoadSPIRVShader(const char *fname, GLenum type) {
	std::ifstream file(fname, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open SPIR-V shader file: " << fname << "\n";
		return 0;
	}

	// Get file size
	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// SPIR-V is a binary format with 4-byte words
	if (fileSize % 4 != 0) {
		std::cerr << "Invalid SPIR-V file: " << fname << " (size not multiple of 4)\n";
		return 0;
	}

	// Read the SPIR-V binary
	std::vector<uint32_t> spirvData(fileSize / 4);
	file.read(reinterpret_cast<char*>(spirvData.data()), fileSize);

	// Create and specify shader
	GLuint shader = glCreateShader(type);

	// Use glShaderBinary if SPIR-V extension is available
#ifdef GL_ARB_gl_spirv
	// Check if SPIR-V is supported
	if (glewIsSupported("GL_ARB_gl_spirv")) {
		glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirvData.data(), fileSize);
		glSpecializeShaderARB(shader, "main", 0, nullptr, nullptr);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar infoLog[512];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "SPIR-V specialization failed (" << fname << "):\n" << infoLog << "\n";
			return 0;
		}
	} else {
		std::cerr << "SPIR-V not supported by this GL implementation, falling back to GLSL.\n";
		glDeleteShader(shader);
		return 0;
	}
#else
	std::cerr << "SPIR-V support not compiled in, falling back to GLSL.\n";
    glDeleteShader(shader);
    return 0;
#endif

	return shader;
}

void Shader::Use() const {
	glUseProgram(progId);
}

void Shader::SetMVP(const float *mvp, const float *mv) const {
	if (mvpId != -1 && mvp) {
		glUniformMatrix4fv(mvpId, 1, GL_TRUE, mvp);
	}
	if (mvId != -1 && mv) {
		glUniformMatrix4fv(mvId, 1, GL_TRUE, mv);
	}
}