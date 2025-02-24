#pragma once

#include "LevelConfig.h"
#include <unordered_map>

class LevelManager {
public:
	void RegisterLevel(const std::string &name, const std::string &yamlPath);

	LevelConfig LoadConfig(const std::string &levelName);

private:
	std::unordered_map<std::string, std::string> levelPaths;
};
