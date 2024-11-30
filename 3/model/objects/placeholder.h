#pragma once

#include "../interfaces.h"
#include "../system/environment.h"

class Placeholder :
    public IRandomMovement {
protected:
    Pair position_ = {0, 0};
    Environment* environment_ = nullptr;
    int speed_ = 0;
    
    Placeholder(Pair position, Environment* environment, int speed)
        : position_(position), environment_(environment), speed_(speed) {}

public:
    virtual ~Placeholder() = default;
    
    Environment* getEnvironment() const {return environment_;}
    Pair getPosition() const {return position_;}
    void setPosition(Pair position) {position_ = position;}
    int getSpeed() const {return speed_;}
    
    virtual void setSpeed(int speed) = 0; 
    void move(Pair position) override;
    Pair calculateRandomMove() const override;
    
    virtual void update() = 0;   
};