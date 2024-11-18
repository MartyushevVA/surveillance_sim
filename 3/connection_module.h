#pragma once

#include "module.h"
#include "interfaces_fromModules.h"
#include <vector>
#include <algorithm>

class ConnectionModule :
    public Module,
    public IConnection {
protected:
    int maxSessions_ = 5;
    std::vector<ConnectionModule*> sessionList_ {};
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

    std::vector<ConnectionModule*> scanForModules() override;

    bool establishConnection(ConnectionModule* module, bool isResponse) override;
    bool closeConnection(ConnectionModule* module) override;

    std::vector<routeNode> getRouteList() const override {return routeList_;}
    std::vector<routeNode> requestRouteList() const override;
    void applyRouteList(std::vector<routeNode> routeList) override;

    void attachTo(Platform* host) const override;
};
/*
sessionList consists of pointers to net modules which can be connected
to current module directly, at first knee :)

routeList consists of pointers to every possible for connection modules
even if it cant be connected straightforward and index of certain connection
from connection table where it need to send package to reach desired destination.
There is some kind of recursion. It now has gate pointer instead of index.
*/