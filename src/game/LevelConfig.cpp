#include "game/LevelConfig.h"
#include <fstream>
#include <iostream>

#include "core/engine/GameHeader.h"

LevelConfig::LevelConfig(const std::string &yamlPath) {
    YAML::Node root = YAML::LoadFile(yamlPath);
    ParseYAML(root);
}

void LevelConfig::ParseYAML(const YAML::Node &root) {
    name = root["name"].as<std::string>("Unnamed Level");

    // Player start con valori di default
    if(root["player_start"]) {
        player_start = root["player_start"].as<std::vector<float>>();
    } else {
        player_start = {0.0f, GH_PLAYER_HEIGHT, 0.0f}; // Valore di default sensato
    }

    for (const auto &node: root["objects"]) {
        ObjectConfig obj;

        // Campo type obbligatorio
        if(!node["type"]) {
            std::cerr << "Oggetto senza tipo, saltato\n";
            continue;
        }
        obj.type = node["type"].as<std::string>();

        // Subtype con default
        obj.subtype = node["subtype"].as<std::string>("");

        // ID con default
        obj.id = node["id"].as<std::string>("");

        // Position con default
        if (node["position"]) {
            obj.position = node["position"].as<std::vector<float>>();
        } else {
            obj.position = {0.0f, 0.0f, 0.0f};
        }

        // Scale con controllo dimensioni
        if (node["scale"]) {
            if (node["scale"].IsScalar()) {
                auto scale = node["scale"].as<float>(1.0f);
                obj.scale = {scale, scale, scale};
            } else {
                auto vec_scale = node["scale"].as<std::vector<float>>();
                if(vec_scale.size() == 1) {
                    obj.scale = {vec_scale[0], vec_scale[0], vec_scale[0]};
                } else if(vec_scale.size() >= 3) {
                    obj.scale = {vec_scale[0], vec_scale[1], vec_scale[2]};
                } else {
                    std::cerr << "Scala non valida per oggetto " << obj.id
                            << ", usando default (1,1,1)\n";
                    obj.scale = {1.0f, 1.0f, 1.0f};
                }
            }
        } else {
            obj.scale = {1.0f, 1.0f, 1.0f};
        }

        // Rotation con default
        if (node["rotation"]) {
            auto rot = node["rotation"].as<std::vector<float>>();
            if(rot.size() >= 3) {
                obj.rotation = {rot[0], rot[1], rot[2]};
            }
        } else {
            obj.rotation = {0.0f, 0.0f, 0.0f};
        }

        // Portals con controllo
        if (node["portals"]) {
            obj.portals = node["portals"].as<std::vector<YAML::Node>>();
        }

        objects.push_back(obj);
    }
}
