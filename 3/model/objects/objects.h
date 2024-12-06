#pragma once

#include "base_platform.h"

class StaticPlatform :
    public Platform {
public:
    StaticPlatform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount)
        : Platform(position, environment, description, maxEnergyLevel, slotCount) {}
};

class MobilePlatform :
    public Platform,
    public IMoving {
public:
    MobilePlatform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount, int speed)
        : Platform(position, environment, description, maxEnergyLevel, slotCount), IMoving(speed) {}

    int getSpeed() const override {return speed_;}
    void setSpeed(int speed) override {speed_ = speed;}
    void move(Pair position) override;
    Pair opponentBasedMove(Pair opponent) const override;
    Pair randomMove() const override;

    void iterate(std::vector<Placeholder*> spottedSuspects);
};

class Suspect : 
    public Placeholder,
    public IMoving {
private:
    int sensorRange_ = 0;

public:
    Suspect(Pair position, Environment* environment, int speed, int sensorRange)
        : Placeholder(position, environment), IMoving(speed), sensorRange_(sensorRange) {}

    int getSpeed() const override {return speed_;}
    void setSpeed(int speed) override {speed_ = speed;}
    void move(Pair position) override;
    Pair opponentBasedMove(Pair opponent) const override;
    Pair randomMove() const override;

    void iterate();

    Platform* nearestPredatorWithinRange() const;
};

class Obstacle : public Placeholder {
public:
    Obstacle(Pair position, Environment* environment) 
        : Placeholder(position, environment) {}
};