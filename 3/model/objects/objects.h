#pragma once

#include "placeholder.h"
#include "base_platform.h"

class StaticPlatform :
    public Placeholder,
    public Platform {
public:
    StaticPlatform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount)
        : Placeholder(position, environment), Platform(description, maxEnergyLevel, slotCount) {}

    Environment* getEnvironment() const override {return environment_;}
    Pair getPosition() const override {return position_;}

    void iterate(std::vector<Placeholder*>) override;
};

class MobilePlatform :
    public MovablePlaceholder,
    public Platform {
public:
    MobilePlatform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount, int speed)
        : MovablePlaceholder(position, environment, speed), Platform(description, maxEnergyLevel, slotCount) {}

    Environment* getEnvironment() const override {return environment_;}
    Pair getPosition() const override {return position_;}

    Pair opponentBasedMove(Pair opponent) const override;

    void iterate(std::vector<Placeholder*> spottedSuspects) override;
};

class Suspect : 
    public MovablePlaceholder,
    public ISensor {
private:
    int visionRange_ = 0;
    Report getSurrounding() const override;

public:
    Suspect(Pair position, Environment* environment, int speed, int visionRange)
        : MovablePlaceholder(position, environment, speed), visionRange_(visionRange) {}

    Pair opponentBasedMove(Pair opponent) const override;

    void iterate();
    
    std::shared_ptr<Placeholder> getNearestVisibleOpponent() const override;
};

class Obstacle : public Placeholder {
public:
    Obstacle(Pair position, Environment* environment) 
        : Placeholder(position, environment) {}
};