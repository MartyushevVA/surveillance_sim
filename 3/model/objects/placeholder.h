#pragma once

#include <memory>
#include <shared_mutex>
#include <mutex>

#include "../interfaces.h"
#include "../system/environment.h"

class Placeholder {
protected:
    Pair position_ = {0, 0};
    std::weak_ptr<Environment> environment_ {};

    mutable std::shared_mutex mutex_;
    
    Placeholder(Pair position, std::weak_ptr<Environment> environment)
        : position_(position), environment_(environment) {}

public:
    virtual ~Placeholder() = default;
    
    std::weak_ptr<Environment> getEnvironment() const {return environment_;}
    Pair getPosition() const {return position_;}
    void setPosition(Pair position) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        position_ = position;
    }
    std::shared_mutex& getMutex() { return mutex_; }
};

class MovablePlaceholder :
    public Placeholder,
    public IMoving {
protected:
    int speed_ = 0;

    MovablePlaceholder(Pair position, std::weak_ptr<Environment> environment, int speed)
        : Placeholder(position, environment), speed_(speed) {}
public:
    virtual ~MovablePlaceholder() = default;

    int getSpeed() const override {return speed_;}
    void setSpeed(int speed) override {speed_ = speed;}
    void move(Pair position) override;
    Pair randomMove() const override;
};
