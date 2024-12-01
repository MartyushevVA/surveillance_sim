#pragma once

#include "base_platform.h"

class StaticPlatform :
    public Platform {
public:
    StaticPlatform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount)
        : Platform(position, environment, description, maxEnergyLevel, slotCount, 0) {}

    void setSpeed(int) override {speed_ = 0;}
    void move(Pair position) override { Placeholder::move(position); }
};

class MobilePlatform :
    public Platform,
    public IPursuitMovement {
public:
    MobilePlatform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount, int speed)
        : Platform(position, environment, description, maxEnergyLevel, slotCount, speed) {}

    void setSpeed(int speed) override {speed_ = speed;}
    void move(Pair position) override { Placeholder::move(position); }
    Pair calculatePursuitMove(Pair target) const override;
};

class Suspect : 
    public Placeholder,
    public IAvoidanceMovement {
private:
    int sensorRange_ = 0;

public:
    Suspect(Pair position, Environment* environment, int speed, int sensorRange)
        : Placeholder(position, environment, speed), sensorRange_(sensorRange) {}

    void setSpeed(int speed) override {speed_ = speed;}
    void move(Pair position) override { Placeholder::move(position); }
    Pair calculateAvoidanceMove(Pair threat) const override;

    void update() override {}

    Platform* nearestPredatorWithinRange() const;
};

class Obstacle : public Placeholder {
public:
    Obstacle(Pair position, Environment* environment) 
        : Placeholder(position, environment, 0) {}
        
    void setSpeed(int) override {}
    void update() override {}
};