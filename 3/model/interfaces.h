#pragma once

#include "common_types.h" 

class IConnection {
public:
    virtual ~IConnection() = default;

    virtual bool establishConnection(std::weak_ptr<ConnectionModule> module, bool isResponse = false) = 0;
    virtual bool closeConnection(std::weak_ptr<ConnectionModule> module, bool isResponse = false) = 0;
};

class IWeapon {
public:
    virtual ~IWeapon() = default;
    virtual bool attack(Pair target) = 0;
};

class ISensor {
protected:
    virtual Report getSurrounding() const = 0;

public:
    virtual ~ISensor() = default;
    virtual std::shared_ptr<Placeholder> getNearestVisibleOpponent() const = 0;
};

class IMoving {
public:    
    virtual ~IMoving() = default;
    virtual int getSpeed() const = 0;
    virtual void setSpeed(int speed) = 0;
    virtual void move(Pair position) = 0;
    virtual Pair opponentBasedMove(Pair opponent) const = 0;
    virtual Pair randomMove() const = 0;
};