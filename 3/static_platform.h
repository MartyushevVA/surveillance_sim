#pragma once

#include "platform.h"

class StaticPlatform : 
    public Placeholder,
    public Platform {
public:
    StaticPlatform() = default;
    StaticPlatform(Pair position, Environment* environment, std::string description, int energyLevel, int slotCount)
        : Platform(description, energyLevel, slotCount), Placeholder(position, environment) {}
};