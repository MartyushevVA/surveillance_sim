#pragma once

#include <random>

#include "interfaces.h"

class Environment;

class Intruder : 
    public Placeholder,
    public IAvoidanceMovement {
public:
    Intruder() = default;
    Intruder(Pair position, Environment* environment, int speed)
        : Placeholder(position, environment, speed) {}

    void setSpeed(int speed) override {if (speed > 0) speed_ = speed;}

    Pair calculateAvoidanceMove(Pair threat) const override;
};