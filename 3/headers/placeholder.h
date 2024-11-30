#pragma once

#include "interfaces.h"

class Environment;

class Placeholder : public IRandomMovement {
protected:
    Pair position_ = {0, 0};
    int speed_ = 0;
    Environment* environment_ = nullptr;
    Placeholder(Pair position, Environment* environment, int speed)
        : position_(position), environment_(environment), speed_(speed) {}

public:
    virtual ~Placeholder() = default;

    Pair getPosition() const {return position_;}
    void setPosition(Pair position) {position_ = position;}
    int getSpeed() const {return speed_;}
    virtual void setSpeed(int speed) = 0;

    virtual void update() = 0;
    
    void move(Pair position) override;
    Pair calculateRandomMove() const override;

    Environment* getEnvironment() const {return environment_;}
};