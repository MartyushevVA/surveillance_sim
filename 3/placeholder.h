#pragma once

#include "environment.h"

struct Pair {
    int x, y;
};

class Placeholder {
protected:
    Pair position_ = {0, 0};
    Environment* environment_ = nullptr;
    Placeholder() = default;
    Placeholder(Pair position, Environment* environment) : position_(position), environment_(environment) {}

public:
    virtual ~Placeholder() = default;

    Pair getPosition() const {return position_;}
    void setPosition(Pair position) {position_ = position;}
    
    Environment* getEnvironment() const {return environment_;}
};