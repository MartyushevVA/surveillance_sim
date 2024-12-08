#pragma once

#include <chrono>

#include "base_module.h"
#include "../interfaces.h"

class StaticPlatform;

class ConnectionModule :
    public Module,
    public IConnection {
private:
    int maxSessions_ = 5;
    std::vector<ConnectionModule*> sessionList_ {};
    std::vector<routeNode> routeList_ {};

public:
    ConnectionModule(int slotsOccupied, int energyConsumption, int range, int maxSessions)
        : Module(slotsOccupied, energyConsumption, range), maxSessions_(maxSessions) {}

    int getMaxSessions() const {return maxSessions_;}
    void setMaxSessions(int maxSessions) {maxSessions_ = maxSessions;}
    std::vector<ConnectionModule*> getSessionList() const {return sessionList_;}
    std::vector<routeNode> getRouteList() const {return routeList_;}
    
    std::vector<ConnectionModule*> scanForModules(Pair position) const;

    bool establishConnection(ConnectionModule* module, bool isResponse = false) override;
    bool closeConnection(ConnectionModule* module, bool isResponse = false) override;
    
    std::vector<routeNode> requestRouteList(ConnectionModule* source) const;
    
    void recursiveRouteNodeImplementation(ConnectionModule* gate, std::vector<routeNode> routeList);
    void recursiveDiscord(ConnectionModule* gate, std::vector<routeNode> targetList);

    bool connectedToAI(const ConnectionModule* source) const;
    bool isSafeForSystem(Pair position) const;
    StaticPlatform* getConnectedToAIDirectly() const;

    void turnOn() override;
    void turnOff() override;

    void update() override;
};

class SensorModule :
    public Module,
    public ISensor {
private:
    SensorType type_ = SensorType::Optical;
    Report getSurrounding() const override;

public:
    SensorModule(int slotsOccupied, int energyConsumption, int range, SensorType type)
        : Module(slotsOccupied, energyConsumption, range), type_(type) {}

    SensorType getType() const {return type_;}
    void setType(SensorType type) {type_ = type;}

    std::shared_ptr<Placeholder> getNearestVisibleOpponent() const override;
    std::map<Pair, std::shared_ptr<Placeholder>> getSuspects() const;

    void turnOn() override;
    void turnOff() override;

    void update() override {};
};

class WeaponModule :
    public Module,
    public IWeapon {
private:
    std::chrono::steady_clock::time_point chargingStarted_;
    std::chrono::milliseconds chargingDuration_;
    bool isCharging_ = false;
    bool isCharged_ = false;
    
    void startCharging();
    void stopCharging();

public:
    WeaponModule(int slotsOccupied, int energyConsumption, int range, std::chrono::milliseconds chargingDuration)
        : Module(slotsOccupied, energyConsumption, range), chargingDuration_(chargingDuration), isCharging_(false), isCharged_(false) {}
    
    std::chrono::milliseconds getChargingDuration() const {return chargingDuration_;}
    void setChargingDuration(std::chrono::milliseconds chargingDuration) {chargingDuration_ = chargingDuration;}
    bool getIsCharging() const {return isCharging_;}
    void setIsCharging(bool isCharging) {isCharging_ = isCharging;}
    bool getIsCharged() const {return isCharged_;}
    void setIsCharged(bool isCharged) {isCharged_ = isCharged;}

    bool attack(Pair suspect) override;

    void turnOn() override;
    void turnOff() override;

    void update() override;
};