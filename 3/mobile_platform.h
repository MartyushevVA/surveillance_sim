#pragma once

#include "platform.h"
#include "moving_object.h"
#include "interfaces_move.h"

class MobilePlatform :
    public Platform,
    public MovingObject,
    public IPursuitMovement {
public:
    MobilePlatform() = default;
    MobilePlatform(Pair position, Environment* environment, std::string description, int energyLevel, int slotCount, int speed)
        : Platform(description, energyLevel, slotCount), MovingObject(position, environment, speed) {}

    Pair calculatePursuitMove(Pair target) const override;
};