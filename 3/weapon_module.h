#pragma once

#include <chrono>
#include "module.h"
#include "interfaces_fromModules.h"

class Intruder;

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
};