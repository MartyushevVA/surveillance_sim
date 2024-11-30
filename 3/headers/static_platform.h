#pragma once

#include "platform.h"

class Environment;

class StaticPlatform : public Platform {
public:
    StaticPlatform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount)
        : Platform(position, environment, description, maxEnergyLevel, slotCount, 0) {}

    void setSpeed(int speed) override {speed_ = 0;}

    void move(Pair position) override { Placeholder::move(position); }
};