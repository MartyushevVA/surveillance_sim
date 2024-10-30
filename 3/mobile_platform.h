#pragma once

#include "platform.h"
#include "moving_object.h"
#include "move.h"

class MobilePlatform :
    public Platform,
    public MovingObject,
    public IRandomMovement,
    public IPursuitMovement {
public:
    MobilePlatform() : Platform{}, MovingObject{} {}
    MobilePlatform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount, int speed)
        : Platform(x, y, environment, description, energyLevel, slotCount), MovingObject(speed) {}

    Pair calculateRandomMove() const override;
    Pair calculatePursuitMove(Placeholder* target) const override;
};