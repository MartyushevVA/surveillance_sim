#pragma once

#include "placeholder.h"

class MovingObject {
protected:
    int speed_ = 0;
    MovingObject() {}
    MovingObject(int speed) : speed_(speed) {};
public:
    void setSpeed(int speed);
    int getSpeed() const;

    virtual void updatePosition() = 0;
};