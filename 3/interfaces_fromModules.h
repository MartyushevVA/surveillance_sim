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
    
    virtual void implementRouteNodes(std::vector<routeNode> routeNodes) = 0; //adjust new data to actual route list;
    
    virtual void sendInfo(std::string info) = 0; //sending and receiving messages (in advance more complex than just string);
    virtual std::string receiveInfo() = 0;

    virtual void establishConnection(Module& module) = 0; //establishing connection with other module;
    virtual void closeConnection(Module& module) = 0; //closing connection with other module;

    virtual bool isConnectedToAI() const = 0; //checking if connection with AI is established (recursive);
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
