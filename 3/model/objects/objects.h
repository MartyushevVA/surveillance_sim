#pragma once

#include <memory>
#include "../common_types.h"
#include "base_platform.h"
#include "placeholder.h"

class Environment;
class AI;

#include "../system/ai.h"
#include "../system/environment.h"

class StaticPlatform :
    public Placeholder,
    public Platform {

private:
    std::weak_ptr<AI> ai_ {};

public:
    StaticPlatform(Pair position, std::weak_ptr<Environment> environment, std::string description, int maxEnergyLevel, int slotCount)
        : Placeholder(position, environment), Platform(description, maxEnergyLevel, slotCount) {}

    std::weak_ptr<Environment> getEnvironment() const override {return Placeholder::getEnvironment();}
    Pair getPosition() const override {return position_;}

    void setAI(std::weak_ptr<AI> ai) {ai_ = ai;}
    std::weak_ptr<AI> getAI() const override {return ai_;}

    void iterate() override;

    void pause() override {Platform::pause();}
    void resume() override {Platform::resume();}
};

class MobilePlatform :
    public MovablePlaceholder,
    public Platform {
public:
    MobilePlatform(Pair position, std::weak_ptr<Environment> environment, std::string description, int maxEnergyLevel, int slotCount, int speed)
        : MovablePlaceholder(position, environment, speed), Platform(description, maxEnergyLevel, slotCount) {}

    std::weak_ptr<Environment> getEnvironment() const override {return Placeholder::getEnvironment();}
    Pair getPosition() const override {return position_;}

    Pair opponentBasedMove(Pair opponent) const override;

    std::weak_ptr<AI> getAI() const override {return {};}
    
    void iterate() override;

    void pause() override {Platform::pause();}
    void resume() override {Platform::resume();}
};

class Suspect : 
    public MovablePlaceholder,
    public ISensor {
private:
    int visionRange_ = 0;
    Report getSurrounding() const override;

public:
    Suspect(Pair position, std::weak_ptr<Environment> environment, int speed, int visionRange)
        : MovablePlaceholder(position, environment, speed), visionRange_(visionRange) {}

    Pair opponentBasedMove(Pair opponent) const override;

    void iterate();

    void pause() override {};
    void resume() override {};
};

class Obstacle : public Placeholder {
public:
    Obstacle(Pair position, std::weak_ptr<Environment> environment) 
        : Placeholder(position, environment) {}

    void pause() override {};
    void resume() override {};
};