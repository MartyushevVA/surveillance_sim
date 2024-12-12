#pragma once

#include <chrono>

#include "base_module.h"
#include "../interfaces.h"
#include "myvector/myvector.h"
class StaticPlatform;
class Suspect;

class ConnectionModule :
    public Module,
    public IConnection,
    public std::enable_shared_from_this<ConnectionModule> {
private:
    int maxSessions_ = 5;
    Vector<std::weak_ptr<ConnectionModule>> sessionList_ {};
    Vector<routeNode> routeList_ {};
    std::shared_mutex mutex_;

public:
    ConnectionModule(int slotsOccupied, int energyConsumption, int range, int maxSessions)
        : Module(slotsOccupied, energyConsumption, range), maxSessions_(maxSessions) {}

    int getMaxSessions() const {return maxSessions_;}
    void setMaxSessions(int maxSessions) {maxSessions_ = maxSessions;}
    Vector<std::weak_ptr<ConnectionModule>> getSessionList() const {return sessionList_;}
    Vector<routeNode> getRouteList() const {return routeList_;}
    
    Vector<std::weak_ptr<ConnectionModule>> scanForModules(Pair position) const;

    bool establishConnection(std::weak_ptr<ConnectionModule> module, bool isResponse = false) override;
    bool closeConnection(std::weak_ptr<ConnectionModule> module, bool isResponse = false) override;
    
    Vector<routeNode> requestRouteList(std::weak_ptr<ConnectionModule> source) const;

    void recursiveRouteNodeImplementation(std::weak_ptr<ConnectionModule> gate, Vector<routeNode> routeList);
    void recursiveDiscord(std::weak_ptr<ConnectionModule> gate, Vector<routeNode> targetList);

    bool isGateToAI(std::weak_ptr<ConnectionModule> gate) const;
    bool isSafeForSystem(Pair position) const;
    std::weak_ptr<const ConnectionModule> getConnectedToAIDirectly() const;

    std::shared_mutex& getMutex() {return mutex_;}

    void turnOn() override;
    void turnOff() override;

    void update() override;
};

class SensorModule :
    public Module,
    public ISensor {
private:
    SensorType type_ = SensorType::Optical;

public:
    SensorModule(int slotsOccupied, int energyConsumption, int range, SensorType type)
        : Module(slotsOccupied, energyConsumption, range), type_(type) {}

    SensorType getType() const {return type_;}
    void setType(SensorType type) {type_ = type;}

    Report getSurrounding() const override;

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

    void startCharging();
    void stopCharging();

    void turnOn() override;
    void turnOff() override;

    void update() override;
};