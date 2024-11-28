#pragma once

#include "placeholder.h"
#include "interfaces.h"

class Suspect : 
    public Placeholder,
    public IAvoidanceMovement {
private:
    int sensorRange_ = 0;

public:
    Suspect(Pair position, Environment* environment, int speed, int sensorRange)
        : Placeholder(position, environment, speed), sensorRange_(sensorRange) {}

    void setSpeed(int speed) override {
        if (speed <= 0)
            throw std::invalid_argument("Speed must be positive");
        speed_ = speed;
    }

    void update() override {}

    Platform* nearestPredatorWithinRange() const;

    void move(Pair position) override { Placeholder::move(position); }

    Pair calculateAvoidanceMove(Pair threat) const override;
};