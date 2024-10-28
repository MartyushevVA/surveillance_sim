#pragma once

#include "placeholder.h"
#include "moving_object.h"

class Environment;

class Intruder : public Placeholder, public MovingObject {
public:
    Intruder() : Placeholder{}, MovingObject{} {}
    Intruder(int x, int y, Environment* environment, int speed)
        : Placeholder({x, y}, environment), MovingObject(speed) {}

    void updatePosition() override;
};