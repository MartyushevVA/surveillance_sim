#pragma once

#include "module.h"
#include <vector>

struct routeNode {
    std::shared_ptr<Module> destination;
    std::shared_ptr<Module> gate;
};

class ConnectionModule : public Module {
private:
    int maxSessions_ = 5;
    std::vector<std::shared_ptr<Module>> sessionList_ = {};
    std::vector<routeNode> routeList_ = {};

public:
    ConnectionModule() : Module{} {}
    ConnectionModule(int slotsOccupied, int energyConsumption, bool isOn, int range, int maxSessions)
        : Module(slotsOccupied, energyConsumption, isOn, range), maxSessions_(maxSessions) {}

    int getMaxSessions() const;
    void setMaxSessions(int maxSessions);
    std::vector<std::shared_ptr<Module>> getSessionList() const;
    void setSessionList(std::vector<std::shared_ptr<Module>> sessionList);
    std::vector<routeNode> getRouteList() const;
    void setRouteList(std::vector<routeNode> routeList);

    void scanForRouteNodes();
    void askForRouteNodesOfRelatives(); // expand actual list;
    // getter for sharing with other modules;

    void establishConnection(Module* module);
    void closeConnection(Module* module);
};

/*
sessionList consists of pointers to net modules which can be connected
to current module directly, at first knee :)

routeList consists of pointers to every possible for connection modules
even if it cant be connected straightforward and index of certain connection
from connection table where it need to send package to reach desired destination.
There is some kind of recursion.
*/