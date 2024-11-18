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
    SensorModule() = default;
    SensorModule(int slotsOccupied, int energyConsumption, bool isOn, int range, SensorType type)
        : Module(slotsOccupied, energyConsumption, isOn, range), type_(type) {}

    SensorType getType() const {return type_;}
    void setType(SensorType type) {type_ = type;}

    std::vector<std::shared_ptr<Placeholder>> getSurrounding() const override;
    void attachTo(Platform* host) const override;
};


class WeaponModule :
    public Module,
    public IWeapon {
protected:
    std::chrono::steady_clock::time_point chargingStarted_ = std::chrono::steady_clock::now();
    std::chrono::seconds chargingDuration_ = std::chrono::seconds(0);
    bool isCharging_ = false;
    bool isCharged_ = false;

public:
    WeaponModule() = default;
    WeaponModule(int slotsOccupied, int energyConsumption, bool isOn, int range, std::chrono::seconds chargingDuration, bool isCharging, bool isCharged)
        : Module(slotsOccupied, energyConsumption, isOn, range), chargingDuration_(chargingDuration), isCharging_(isCharging), isCharged_(isCharged) {}
    
    std::chrono::seconds getChargingDuration() const {return chargingDuration_;}
    void setChargingDuration(std::chrono::seconds chargingDuration) {
        if (chargingDuration > std::chrono::seconds(0))
            chargingDuration_ = chargingDuration;
    }
    bool getIsCharging() const {return isCharging_;}
    void setIsCharging(bool isCharging) {isCharging_ = isCharging;}
    bool getIsCharged() const {return isCharged_;}
    void setIsCharged(bool isCharged) {isCharged_ = isCharged;}

    void attack(Pair intruder) override;
    void refreshState();
    void startCharging();

    void attachTo(Platform* host) const override;
};