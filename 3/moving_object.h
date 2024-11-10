#pragma once

#include "interfaces_move.h"
#include "placeholder.h"

class MovingObject : 
    virtual public Placeholder, 
    public IRandomMovement {
protected:
    int speed_ = 0;
    MovingObject() = default;
    MovingObject(int x, int y, Environment* environment, int speed) : Placeholder({x, y}, environment), speed_(speed) {}
public:
    virtual ~MovingObject() = default;

    void setSpeed(int speed) {
        if (speed > 0)
            speed_ = speed;
    }
    int getSpeed() const {return speed_;}

    void move(Pair nextPosition) override;
    bool abilityToMove(Pair position) const override;
    Pair calculateRandomMove() const override;
};