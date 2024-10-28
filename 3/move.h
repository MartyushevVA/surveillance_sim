#pragma once

#include "placeholder.h"

class MobilePlatform;
class Intruder;

class IMove {
public:
    virtual Pair getNextPosition(const Environment* environment, Pair currentPosition) = 0;
};

class IntruderMovement : public IMove {
public:
    Pair getNextPosition(const Environment* environment, Pair currentPosition) override;
    Pair escapeFromAttacker(const Environment* environment, Pair currentPosition, Pair attackerPosition);
};

class AttackerMovement : public IMove {
public:
    Pair getNextPosition(const Environment* environment, Pair currentPosition) override;
    Pair moveTowardsIntruder(const Environment* environment, Pair currentPosition, Pair intruderPosition);
};