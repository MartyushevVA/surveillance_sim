#pragma once

#include <utility>
#include "environment.h"

class IntruderBehavior {
public:
    virtual std::pair<int, int> getNextPosition(const Environment& environment) = 0;
};

class Intruder {
private:
    int x_;
    int y_;
    IntruderBehavior* behavior_;
public:
    Intruder() = default;
    Intruder(int x, int y, IntruderBehavior* behavior);
    ~Intruder() = default;

    void setPosition(int x, int y);
    std::pair<int, int> getPosition() const;
    void setBehavior(IntruderBehavior behavior);
    IntruderBehavior getBehavior(const Environment& environment);

    void changePosition();
};