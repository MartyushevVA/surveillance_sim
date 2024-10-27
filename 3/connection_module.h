#pragma once

#include "module.h"

class ConnectionModule : public Module {
private:
    int maxSessions_ = 5;
    std::vector<Module*> activeSessions_ = {};
    std::vector<Module*> MACs_ = {};

public:
    ConnectionModule() : Module{} {}
    ConnectionModule(int slotsOccupied, int energyConsumption, bool isOn, int range, Platform* host, int maxSessions)
        : Module(slotsOccupied, energyConsumption, isOn, range, host), maxSessions_(maxSessions) {}

    int getMaxSessions() const;
    void setMaxSessions(int maxSessions);
    std::vector<Module*> getAbleToConnect() const;
    void setAbleToConnect(std::vector<Module*> ableToConnect);
    std::vector<Module*> getMACs() const;
    void setMACs(std::vector<Module*> MACs);

    void scanForMACs();
    void askForMACsOfRelatives(); //expand actuall list;
    //getter for sharing with other modules;

    void establishConnection(Module* module);
    void closeConnection(Module* module);
};