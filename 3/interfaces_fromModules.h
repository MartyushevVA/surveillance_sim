#pragma once

#include <memory>
#include <vector>
#include "placeholder.h"

class Module;

struct routeNode {
    Module* destination;
    Module* gate;
};

class IConnection {
public:
    virtual ~IConnection() = default;

    virtual std::vector<Module*> scanForModules() = 0; //simply scans space nearby;
    

    virtual bool establishConnection(ConnectionModule* module, bool isResponse) = 0; //establishing connection with other module;
};

class IWeapon {
public:
    virtual ~IWeapon() = default;
    virtual void attack(Pair target) = 0;
};

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual std::vector<std::shared_ptr<Placeholder>> getSurrounding() const = 0;
};
