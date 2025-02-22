#include "game/LevelManager.h"

void LevelManager::RegisterLevel(const std::string& name, const std::string& yamlPath) {
    levelPaths[name] = yamlPath;
}

LevelConfig LevelManager::LoadConfig(const std::string& levelName) {
    return LevelConfig(levelPaths.at(levelName));
}