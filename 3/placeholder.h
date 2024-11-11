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
    void setPosition(Pair position) {
        if (abilityToMove(position)) {
            position_ = position;
        }
    }
    bool abilityToMove(Pair position) const {
        if (!environment_) return false;
        Pair size = environment_->getSize();
        if (position.x < 0 || position.x >= size.x || 
            position.y < 0 || position.y >= size.y)
            return false;
        return environment_->getCellType(position.x, position.y) == CellType::Empty;
    }
    
    Environment* getEnvironment() const {return environment_;}
};