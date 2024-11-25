#pragma once

#include "platform.h"

class StaticPlatform : public Platform {
public:
    StaticPlatform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount)
        : Platform(position, environment, description, maxEnergyLevel, slotCount, 0) {}

    void setSpeed(int speed) override {speed_ = 0;}
    void positionRelatedUpdate(Pair newPosition = {-1, 0}) override;

    void move(Pair position) override { Placeholder::move(position); }
    bool abilityToMove(Pair position) const override { return Placeholder::abilityToMove(position); }
};