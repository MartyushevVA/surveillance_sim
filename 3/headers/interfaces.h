#pragma once

#include <memory>
#include <vector>

#include "common_types.h" 

class IConnection {
public:
    virtual ~IConnection() = default;

    virtual std::vector<ConnectionModule*> scanForModules(Pair position = {-1, 0}) = 0;

    virtual bool establishConnection(ConnectionModule* module, bool isResponse = false) = 0;
    virtual bool closeConnection(ConnectionModule* module, bool isResponse = false) = 0;

    virtual std::vector<routeNode> getRouteList() const = 0;
    virtual std::vector<routeNode> requestRouteList(ConnectionModule* source) const = 0;
    virtual void applyRouteList(std::vector<routeNode> routeList) = 0;

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

class IMove {
public:
    virtual ~IMove() = default;
    virtual void move(Pair position) = 0;
    virtual bool abilityToMove(Pair position) const = 0;
};

class IRandomMovement : public IMove {
public:
    virtual ~IRandomMovement() = default;
    virtual Pair calculateRandomMove() const = 0;
};

class IPursuitMovement : public IMove {
public:
    virtual ~IPursuitMovement() = default;
    virtual Pair calculatePursuitMove(Pair target) const = 0;
};

class IAvoidanceMovement : public IMove {
public:
    virtual ~IAvoidanceMovement() = default;
    virtual Pair calculateAvoidanceMove(Pair threat) const = 0;
};