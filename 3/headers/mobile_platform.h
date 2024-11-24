#pragma once

#include "platform.h"
#include "interfaces.h"

class MobilePlatform :
    public Platform,
    public IPursuitMovement {
public:
    MobilePlatform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount, int speed)
        : Platform(position, environment, description, maxEnergyLevel, slotCount, speed) {}

    void setSpeed(int speed) override {speed_ = speed;}

    void positionRelatedUpdate(Pair newPosition) override;

    Pair findPursuitableSuspect(Report report) const;

    void move(Pair position) override { Placeholder::move(position); }
    bool abilityToMove(Pair position) const override { return Placeholder::abilityToMove(position); }

    Pair calculatePursuitMove(Pair target) const override;
};