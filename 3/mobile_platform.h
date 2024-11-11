#pragma once

#include "platform.h"
#include "interfaces_move.h"

class MobilePlatform :
    public Platform,
    public IPursuitMovement {
public:
    MobilePlatform() = default;
    MobilePlatform(Pair position, Environment* environment, std::string description, int energyLevel, int slotCount, int speed)
        : Platform(position, environment, description, energyLevel, slotCount, speed) {}

    void setSpeed(int speed) override {
        if (speed > 0)
            speed_ = speed;
    }

    Pair calculatePursuitMove(Pair target) const override;
};