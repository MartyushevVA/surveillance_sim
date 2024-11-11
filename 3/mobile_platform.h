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
    MobilePlatform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount, int speed)
        : Platform(x, y, environment, description, energyLevel, slotCount), MovingObject(speed) {}

   
    Pair calculatePursuitMove(Pair target) const override;
    Pair calculateRandomMove() const override;
};