#pragma once

#include "platform.h"
#include "moving_object.h"

class MobilePlatform : public Platform, public MovingObject{
public:
    MobilePlatform() : Platform{}, MovingObject{} {}
    MobilePlatform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount, int speed)
        : Platform(x, y, environment, description, energyLevel, slotCount), MovingObject(speed) {}

    void updatePosition() override;
};