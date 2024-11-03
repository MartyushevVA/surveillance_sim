#pragma once

#include "platform.h"

class StaticPlatform : public Platform {
public:
    StaticPlatform() = default;
    StaticPlatform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount)
        : Platform(x, y, environment, description, energyLevel, slotCount) {}
};