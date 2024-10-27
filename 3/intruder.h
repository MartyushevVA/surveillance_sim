#pragma once

#include "placeholder.h"
#include <memory>

class Environment;

class IntruderBehavior {
public:
    virtual ~IntruderBehavior() = default;
    virtual Pair getNextPosition(const Environment* environment, Pair currentPosition) = 0;
};

class RandomIntruderBehavior : public IntruderBehavior {
public:
    Pair getNextPosition(const Environment* environment, Pair currentPosition) override;
};

class RunningOutIntruderBehavior : public IntruderBehavior {
public:
    Pair getNextPosition(const Environment* environment, Pair currentPosition) override;
};

class Intruder : public Placeholder {
private:
    std::shared_ptr<IntruderBehavior> behavior;

public:
    Intruder() : Placeholder{} {}
    Intruder(int x, int y, Environment* environment, std::shared_ptr<IntruderBehavior> behavior)
        : Placeholder({x, y}, environment), behavior(std::move(behavior)) {}

    Pair getNextPosition();
    void setBehavior(std::shared_ptr<IntruderBehavior> newBehavior);
    void changePosition();
};