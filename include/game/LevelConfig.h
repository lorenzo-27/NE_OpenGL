#pragma once
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include <string>

struct ObjectConfig {
    std::string type;
    std::string subtype;
    std::string id;
    std::vector<float> position;
    std::vector<float> scale;
    std::vector<float> rotation;
    std::vector<YAML::Node> portals;
};

class LevelConfig {
public:
    explicit LevelConfig(const std::string &yamlPath);

    std::string name;
    std::vector<float> player_start;
    std::vector<ObjectConfig> objects;

private:
    void ParseYAML(const YAML::Node &root);
};
