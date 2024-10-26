#pragma once

#include "environment.h"

class IntruderBehavior {
public:
    virtual Pair getNextPosition(const Environment& environment) = 0;
};

class Intruder : public Placeholder {
private:
    IntruderBehavior* behavior = nullptr;

public:
    Intruder(IntruderBehavior* behavior = nullptr) : Placeholder(), behavior(behavior) {}
    Intruder(int x, int y, IntruderBehavior* behavior = nullptr) : Placeholder({x, y}), behavior(behavior) {}
    ~Intruder();

    void setPosition(int x, int y);
    Pair getPosition() const;
    void setBehavior(IntruderBehavior behavior);
    IntruderBehavior getBehavior(const Environment& environment);

    void changePosition(const Environment& environment);
};