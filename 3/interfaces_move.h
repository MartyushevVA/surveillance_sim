#pragma once

#include "placeholder.h"

class IMove {
public:
    virtual ~IMove() = default;
    virtual void move(Pair nextPosition) = 0;
    virtual bool abilityToMove(Pair position) const = 0;
};

class IRandomMovement : public IMove {
public:
    virtual ~IRandomMovement() = default;
    virtual Pair calculateRandomMove() const = 0;
};

class IPursuitMovement : public IMove {
public:
    virtual ~IPursuitMovement() = default;
    virtual Pair calculatePursuitMove(Pair target) const = 0;
};

class IAvoidanceMovement : public IMove {
public:
    virtual ~IAvoidanceMovement() = default;
    virtual Pair calculateAvoidanceMove(Pair threat) const = 0;
};