#pragma once

#include "placeholder.h"
#include "moving_object.h"
#include "interfaces_move.h"

class Environment;

class Intruder : 
    public MovingObject, 
    public IRandomMovement,
    public IAvoidanceMovement {
public:
    Intruder() = default;
    Intruder(int x, int y, Environment* environment, int speed)
        : Placeholder({x, y}, environment), MovingObject(x, y, environment, speed) {}

    Pair calculateAvoidanceMove(Pair threat) const override;
};