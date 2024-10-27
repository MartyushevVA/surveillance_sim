#pragma once

#include "module.h"
#include <vector>

class ConnectionModule : public Module {
private:
    int maxSessions_ = 5;
    std::vector<std::shared_ptr<Module>> activeSessions_ = {};
    std::vector<std::shared_ptr<Module>> MACs_ = {};

public:
    ConnectionModule() : Module{} {}
    ConnectionModule(int slotsOccupied, int energyConsumption, bool isOn, int range, Platform* host, int maxSessions)
        : Module(slotsOccupied, energyConsumption, isOn, range), maxSessions_(maxSessions) {}

    int getMaxSessions() const;
    void setMaxSessions(int maxSessions);
    std::vector<std::shared_ptr<Module>> getAbleToConnect() const;
    void setAbleToConnect(std::vector<std::shared_ptr<Module>> ableToConnect);
    std::vector<std::shared_ptr<Module>> getMACs() const;
    void setMACs(std::vector<std::shared_ptr<Module>> MACs);

    void scanForMACs();
    void askForMACsOfRelatives(); // expand actual list;
    // getter for sharing with other modules;

    void establishConnection(std::shared_ptr<Module> module);
    void closeConnection(std::shared_ptr<Module> module);
};