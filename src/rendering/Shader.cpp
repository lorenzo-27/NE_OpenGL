#include "rendering/Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *name) : vertId(0), fragId(0), progId(0),
                                  mvpId(-1), mvId(-1) {
    // Get file paths
    std::string vertPath = "assets/shaders/" + std::string(name) + ".vert";
    std::string fragPath = "assets/shaders/" + std::string(name) + ".frag";

    // Load shaders
    vertId = LoadShader(vertPath.c_str(), GL_VERTEX_SHADER);
    fragId = LoadShader(fragPath.c_str(), GL_FRAGMENT_SHADER);

    if(!vertId || !fragId) {
        std::cerr << "Failed to load shaders: " << name << "\n";
        return;
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
    if(!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(progId, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed (" << name << "):\n" << infoLog << "\n";
        return;
    }

    // Get uniform locations
    mvpId = glGetUniformLocation(progId, "mvp");
    mvId = glGetUniformLocation(progId, "mv");
}

Shader::~Shader() {
    glDeleteProgram(progId);
    glDeleteShader(vertId);
    glDeleteShader(fragId);
}

GLuint Shader::LoadShader(const char *fname, GLenum type) {
    std::ifstream file(fname);
    if(!file.is_open()) {
        std::cerr << "Failed to open shader file: " << fname << "\n";
        return 0;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    const char* source = content.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed (" << fname << "):\n" << infoLog << "\n";
        return 0;
    }

    return shader;
}

void Shader::Use() const {
    glUseProgram(progId);
}

void Shader::SetMVP(const float *mvp, const float *mv) const {
    if(mvpId != -1 && mvp) {
        glUniformMatrix4fv(mvpId, 1, GL_TRUE, mvp);
    }
    if(mvId != -1 && mv) {
        glUniformMatrix4fv(mvId, 1, GL_TRUE, mv);
    }
}