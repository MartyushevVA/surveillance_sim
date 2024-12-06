#pragma once

#include "../interfaces.h"
#include "../system/environment.h"

class Placeholder {
protected:
    Pair position_ = {0, 0};
    Environment* environment_ = nullptr;
    
    Placeholder(Pair position, Environment* environment)
        : position_(position), environment_(environment) {}

public:
    virtual ~Placeholder() = default;
    
    Environment* getEnvironment() const {return environment_;}
    Pair getPosition() const {return position_;}
    void setPosition(Pair position) {position_ = position;}
};