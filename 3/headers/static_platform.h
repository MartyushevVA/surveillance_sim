#pragma once

#include "platform.h"

class StaticPlatform : public Platform {
public:
    StaticPlatform(Pair position, Environment* environment, std::string description, int energyLevel, int slotCount)
        : Platform(position, environment, description, energyLevel, slotCount, 0) {}

    void setSpeed(int speed) override {speed_ = 0;}
    void positionRelatedUpdate(Pair newPosition) override {}
};