#pragma once

#include "platform.h"
#include "interfaces.h"

class MobilePlatform :
    public Platform,
    public IPursuitMovement {
public:
    MobilePlatform(Pair position, Environment* environment, std::string description, int energyLevel, int slotCount, int speed)
        : Platform(position, environment, description, energyLevel, slotCount, speed) {}

    void setSpeed(int speed) override {
        if (speed <= 0)
            throw std::invalid_argument("Speed must be positive");
        speed_ = speed;
    }

    void positionRelatedUpdate(Pair newPosition) override;

    Pair findPursuitableSuspect(Report report) const;

    void move(Pair position) override { Placeholder::move(position); }
    bool abilityToMove(Pair position) const override { return Placeholder::abilityToMove(position); }

    Pair calculatePursuitMove(Pair target) const override;
};