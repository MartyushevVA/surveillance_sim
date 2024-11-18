#pragma once

#include "platform.h"
#include "interfaces.h"

class MobilePlatform :
    public Platform,
    public IPursuitMovement {
public:
    MobilePlatform(Pair position, Environment* environment, std::string description, int energyLevel, int slotCount, int speed)
        : Platform(position, environment, description, energyLevel, slotCount, speed) {}

    void setSpeed(int speed) override {
        if (speed <= 0)
            throw std::invalid_argument("Speed must be positive");
        speed_ = speed;
    }

    Pair calculatePursuitMove(Pair target) const override;
};