#pragma once

#include "platform.h"

class StaticPlatform : public Platform {
public:
    StaticPlatform() = default;
    StaticPlatform(int x, int y, const std::string description, int energyLevel, int slotCount, std::vector<Module*> modules = {})
        : Platform(x, y, description, energyLevel, slotCount, modules) {}
};