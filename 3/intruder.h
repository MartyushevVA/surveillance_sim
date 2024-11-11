#pragma once

#include "placeholder.h"
#include "moving_object.h"
#include "interfaces_move.h"

class Environment;

class Intruder : 
    public MovingObject,
    public IAvoidanceMovement {
public:
    Intruder() = default;
    Intruder(Pair position, Environment* environment, int speed)
        : MovingObject(position, environment, speed) {}

    Pair calculateAvoidanceMove(Pair threat) const override;
};