#pragma once

#include "rendering/Mesh.h"
#include "rendering/Texture.h"
#include "rendering/Shader.h"
#include <memory>

std::shared_ptr<Mesh> AcquireMesh(const char *name);

std::shared_ptr<Shader> AcquireShader(const char *name);

std::shared_ptr<Texture> AcquireTexture(const char *name, int rows = 1, int cols = 1, TextureType type = TextureType::DIFFUSE);

void CheckForShaderUpdates(bool forceReload = false);

bool ReloadShader(const char *name);
