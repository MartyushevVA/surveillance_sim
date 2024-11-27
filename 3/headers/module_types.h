#pragma once

#include <algorithm>
#include <chrono>

#include "module.h"
#include "interfaces.h"

class ConnectionModule :
    public Module,
    public IConnection {
protected:
    int maxSessions_ = 5;
    std::vector<ConnectionModule*> sessionList_ {};
    std::vector<routeNode> routeList_ {};

public:
    ConnectionModule(int slotsOccupied, int energyConsumption, bool isOn, int range, int maxSessions)
        : Module(slotsOccupied, energyConsumption, isOn, range), maxSessions_(maxSessions) {}

    int getMaxSessions() const {return maxSessions_;}
    void setMaxSessions(int maxSessions) {
        if (maxSessions <= 0)
            throw std::invalid_argument("Max sessions must be positive");
        maxSessions_ = maxSessions;
    }
    std::vector<ConnectionModule*> getSessionList() const {return sessionList_;}
    std::vector<routeNode> getRouteList() const override {return routeList_;}
    
    std::vector<ConnectionModule*> scanForModules(Pair position = {-1, 0}) override;

    bool establishConnection(ConnectionModule* module, bool isResponse = false) override;
    bool closeConnection(ConnectionModule* module, bool isResponse = false) override;
    
    void applyRouteList(std::vector<routeNode> routeList) override;
    std::vector<routeNode> requestRouteList(ConnectionModule* source) const override;
    
    void recursiveRouteNodeImplementation(ConnectionModule* gate, std::vector<routeNode> routeList);
    void recursiveDiscord(ConnectionModule* gate, std::vector<routeNode> targetList);

    bool attachableTo(std::shared_ptr<Platform> host) const override;

    void update() override;
    void setUp() override;
};
/*
sessionList consists of pointers to net modules which can be connected
to current module directly, at first knee :)

routeList consists of pointers to every possible for connection modules
even if it cant be connected straightforward and index of certain connection
from connection table where it need to send package to reach desired destination.
There is some kind of recursion. It now has gate pointer instead of index.
*/


enum SensorType {
    Optical,
    XRay
};

class SensorModule :
    public Module,
    public ISensor {
protected:
    SensorType type_ = SensorType::Optical;

public:
    SensorModule(int slotsOccupied, int energyConsumption, bool isOn, int range, SensorType type)
        : Module(slotsOccupied, energyConsumption, isOn, range), type_(type) {}

    SensorType getType() const {return type_;}
    void setType(SensorType type) {type_ = type;}

    Report getSurrounding() const override;
    std::shared_ptr<Placeholder> getVisibleSuspect(Report report) const;

    bool attachableTo(std::shared_ptr<Platform> host) const override;

    void update() override;
    void setUp() override;
};

class WeaponModule :
    public Module,
    public IWeapon {
protected:
    std::chrono::steady_clock::time_point chargingStarted_;
    std::chrono::milliseconds chargingDuration_;
    bool isCharging_ = false;
    bool isCharged_ = false;

public:
    WeaponModule(int slotsOccupied, int energyConsumption, bool isOn, int range, std::chrono::milliseconds chargingDuration)
        : Module(slotsOccupied, energyConsumption, isOn, range), chargingDuration_(chargingDuration) {}
    
    std::chrono::milliseconds getChargingDuration() const {return chargingDuration_;}
    void setChargingDuration(std::chrono::milliseconds chargingDuration) {chargingDuration_ = chargingDuration;}
    bool getIsCharging() const {return isCharging_;}
    void setIsCharging(bool isCharging) {isCharging_ = isCharging;}
    bool getIsCharged() const {return isCharged_;}
    void setIsCharged(bool isCharged) {isCharged_ = isCharged;}

    void startCharging();

    bool attack(Pair suspect) override;

    bool attachableTo(std::shared_ptr<Platform> host) const override;

    void update() override;
    void setUp() override;
};