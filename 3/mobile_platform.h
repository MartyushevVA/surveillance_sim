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
    int speed_;

public:
    MobilePlatform(int speed = 0) : Platform{}, speed_(speed) {}
    MobilePlatform(int x, int y, const std::string description, int energyLevel, int slotCount, std::vector<Module*> modules = {}, int speed = 0)
        : Platform(x, y, description, energyLevel, slotCount, modules), speed_(speed) {}
    
    void setSpeed(int speed);
    int getSpeed() const;

    void move(Direction direction);
};