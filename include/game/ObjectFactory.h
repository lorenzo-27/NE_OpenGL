#pragma once
#include "game/objects/base/Object.h"
#include "LevelConfig.h"

class ObjectFactory {
public:
    static std::shared_ptr<Object> Create(const ObjectConfig& config);
};