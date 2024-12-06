#pragma once

#include "common_types.h" 

class IConnection {
public:
    virtual ~IConnection() = default;

    virtual std::vector<ConnectionModule*> scanForModules(Pair position = {-1, 0}) const = 0;

    virtual bool establishConnection(ConnectionModule* module, bool isResponse = false) = 0;
    virtual bool closeConnection(ConnectionModule* module, bool isResponse = false) = 0;

    virtual std::vector<routeNode> getRouteList() const = 0;
    virtual std::vector<routeNode> requestRouteList(ConnectionModule* source) const = 0;
};

class IWeapon {
public:
    virtual ~IWeapon() = default;
    virtual bool attack(Pair target) = 0;
};

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual Report getSurrounding() const = 0;
};

class IMoving {
protected:
    int speed_ = 0;

public:
    IMoving(int speed) : speed_(speed) {}
    virtual ~IMoving() = default;

    virtual int getSpeed() const = 0;
    virtual void setSpeed(int speed) = 0;
    virtual void move(Pair position) = 0;
    virtual Pair opponentBasedMove(Pair opponent) const = 0;
    virtual Pair randomMove() const = 0;
};