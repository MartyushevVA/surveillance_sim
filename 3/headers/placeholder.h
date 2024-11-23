#pragma once

#include "interfaces.h"
#include "environment.h"


struct Pair {
    int x, y;
    bool operator==(const Pair& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Pair& other) const {
        return !(*this == other);
    }
};

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

    virtual void positionRelatedUpdate(Pair newPosition) = 0;

    void move(Pair position) override;
    bool abilityToMove(Pair position) const override;
    Pair calculateRandomMove() const override;

    Environment* getEnvironment() const {return environment_;}
};