#pragma once

#include "platform.h"

class StaticPlatform : public Platform {
public:
    StaticPlatform() : Platform{} {}
    StaticPlatform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount, std::vector<Module*> modules)
        : Platform(x, y, environment, description, energyLevel, slotCount, modules) {}
};