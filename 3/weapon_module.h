#pragma once

#include <chrono>
#include "module.h"
#include "interfaces_fromModules.h"

class Intruder;

class WeaponModule :
    public Module,
    public IWeapon {
protected:
    std::chrono::seconds chargingDuration_ = std::chrono::seconds(0);
    bool isCharging_ = false;
    bool isCharged_ = false;

public:
    WeaponModule() = default;
    WeaponModule(int slotsOccupied, int energyConsumption, bool isOn, int range, std::chrono::seconds chargingDuration, bool isCharging, bool isCharged)
        : Module(slotsOccupied, energyConsumption, isOn, range), chargingDuration_(chargingDuration), isCharging_(isCharging), isCharged_(isCharged) {}
    
    std::chrono::seconds getChargingDuration() const;
    void setChargingDuration(std::chrono::seconds chargingDuration);
    bool getIsCharging() const;
    void setIsCharging(bool isCharging);
    bool getIsCharged() const;
    void setIsCharged(bool isCharged);

    void attack(Placeholder& intruder) override;
};