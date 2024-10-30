#pragma once

#include "placeholder.h"
#include "moving_object.h"
#include "move.h"

class Environment;

class Intruder : 
    public Placeholder, 
    public MovingObject, 
    public IRandomMovement,
    public IAvoidanceMovement {
public:
    Intruder() : Placeholder{}, MovingObject{} {}
    Intruder(int x, int y, Environment* environment, int speed)
        : Placeholder({x, y}, environment), MovingObject(speed) {}

    Pair calculateRandomMove() const override;
    Pair calculateAvoidanceMove(Placeholder* threat) const override;
};