#pragma once

#include <chrono>
#include "module.h"
#include "intruder.h"

class WeaponModule : public Module {
private:
    std::chrono::seconds chargingDuration_ = std::chrono::seconds(0);
    bool isCharging_ = false;
    bool isCharged_ = false;

public:
    WeaponModule() : Module{} {}
    WeaponModule(int slotsOccupied, int energyConsumption, bool isOn, int range, Platform* host, std::chrono::seconds chargingDuration, bool isCharging, bool isCharged)
        : Module(slotsOccupied, energyConsumption, isOn, range, host), chargingDuration_(chargingDuration), isCharging_(isCharging), isCharged_(isCharged) {}
    
    std::chrono::seconds getChargingDuration() const;
    void setChargingDuration(std::chrono::seconds chargingDuration);
    bool getIsCharging() const;
    void setIsCharging(bool isCharging);
    bool getIsCharged() const;
    void setIsCharged(bool isCharged);

    void eliminateIntruder(Intruder* intruder);
};