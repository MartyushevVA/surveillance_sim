#pragma once

#include "platform.h"

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
};

class MobilePlatform : public Platform {
private:
    int speed_ = 0;

public:
    MobilePlatform() : Platform{} {}
    MobilePlatform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount, int speed)
        : Platform(x, y, environment, description, energyLevel, slotCount), speed_(speed) {}
    
    void setSpeed(int speed);
    int getSpeed() const;

    void move(Direction direction);
};