#pragma once

#include "interfaces_move.h"
#include "placeholder.h"

class MovingObject : public IRandomMovement {
protected:
    int speed_ = 0;
    MovingObject() = default;
    MovingObject(int speed) : speed_(speed) {}
public:
    virtual ~MovingObject() = default;

    void setSpeed(int speed) {
        if (speed > 0)
            speed_ = speed;
    }
    int getSpeed() const {return speed_;}
};