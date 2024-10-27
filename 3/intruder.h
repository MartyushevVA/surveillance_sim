#pragma once

#include "environment.h"

class IntruderBehavior {
public:
    virtual Pair getNextPosition(const Environment* environment, Intruder* intruder) = 0;
};

class RandomIntruderBehavior : public IntruderBehavior {
public:
    Pair getNextPosition(const Environment* environment, Intruder* intruder) override;
};

class RunningOutIntruderBehavior : public IntruderBehavior {
public:
    Pair getNextPosition(const Environment* environment, Intruder* intruder) override;
};

class Intruder : public Placeholder {
private:
    IntruderBehavior* behavior = nullptr;

public:
    Intruder() : Placeholder{} {}
    Intruder(int x, int y, Environment* environment, IntruderBehavior* behavior) : Placeholder({x, y}, environment), behavior(behavior) {}
    ~Intruder();

    void setPosition(int x, int y);
    Pair getPosition() const;
    void setBehavior(IntruderBehavior* behavior);
    IntruderBehavior* getBehavior() const;

    void changePosition();
};