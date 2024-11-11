#pragma once

#include "interfaces_move.h"
#include "placeholder.h"

class MovingObject : 
    public Placeholder,
    public IRandomMovement {
protected:
    int speed_ = 0;
    MovingObject() = default;
    MovingObject(Pair position, Environment* environment, int speed)
        : Placeholder(position, environment), speed_(speed) {}
public:
    virtual ~MovingObject() = default;

    void setSpeed(int speed) {
        if (speed > 0)
            speed_ = speed;
    }
    int getSpeed() const {return speed_;}

    void move(Pair position) override;
    bool abilityToMove(Pair position) const override;

    Pair calculateRandomMove() const override;
};