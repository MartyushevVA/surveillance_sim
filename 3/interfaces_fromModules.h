#pragma once

#include <memory>
#include <vector>
#include "placeholder.h"

class Module;

struct routeNode {
    ConnectionModule* destination;
    ConnectionModule* gate;
};

class IConnection {
public:
    virtual ~IConnection() = default;

    virtual std::vector<ConnectionModule*> scanForModules() = 0; //simply scans space nearby;

    virtual bool establishConnection(ConnectionModule* module, bool isResponse) = 0; //establishing connection with other module;
    virtual bool closeConnection(ConnectionModule* module) = 0; //closing connection with other module;
    virtual std::vector<routeNode> getRouteList() const = 0; //getting route list from module;
    virtual std::vector<routeNode> requestRouteList() const = 0; //requesting route list from module;
    virtual void applyRouteList(std::vector<routeNode> routeList) = 0; //applying route list to module;

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
