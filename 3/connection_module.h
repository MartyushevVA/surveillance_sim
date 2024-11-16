#pragma once

#include "module.h"
#include "interfaces_fromModules.h"
#include <vector>

class ConnectionModule :
    public Module,
    public IConnection {
protected:
    int maxSessions_ = 5;
    std::vector<Module*> sessionList_ {};
    std::vector<routeNode> routeList_ {};

public:
    ConnectionModule() = default;
    ConnectionModule(int slotsOccupied, int energyConsumption, bool isOn, int range, int maxSessions)
        : Module(slotsOccupied, energyConsumption, isOn, range), maxSessions_(maxSessions) {}

    int getMaxSessions() const {return maxSessions_;}
    void setMaxSessions(int maxSessions) {
        if (maxSessions > 0)
            maxSessions_ = maxSessions;
    }
    std::vector<Module*> getSessionList() const {return sessionList_;}
    void setSessionList(std::vector<Module*> sessionList) {sessionList_ = sessionList;}
    std::vector<routeNode> getRouteList() const {return routeList_;}
    void setRouteList(std::vector<routeNode> routeList) {routeList_ = routeList;}

    std::vector<Module*> scanForModules() override;
    void implementRouteNodes(std::vector<routeNode> routeNodes) override;
    void establishConnection(Module& module) override;
    void closeConnection(Module& module) override;
    bool isConnectedToAI() const override;
};

/*
sessionList consists of pointers to net modules which can be connected
to current module directly, at first knee :)

routeList consists of pointers to every possible for connection modules
even if it cant be connected straightforward and index of certain connection
from connection table where it need to send package to reach desired destination.
There is some kind of recursion. It now has gate pointer instead of index.
*/