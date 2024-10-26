#pragma once

#include <chrono>
#include "module.h"
#include "intruder.h"

class WeaponModule : public Module {
private:
    std::chrono::milliseconds chargingDuration_;
    int energyConsumption_;

public:
    WeaponModule(std::chrono::milliseconds chargingDuration = std::chrono::milliseconds(0), int energyConsumption = 0) : Module{}, chargingDuration_(chargingDuration), energyConsumption_(energyConsumption) {}
    WeaponModule(int x, int y, int slotsOccupied, bool isOn, int range, std::chrono::milliseconds chargingDuration = std::chrono::milliseconds(0), int energyConsumption = 0)
        : Module(x, y, slotsOccupied, isOn, range), chargingDuration_(chargingDuration), energyConsumption_(energyConsumption) {}
    
    void eliminateIntruder(Intruder* intruder);
    void chargingOn();
    void chargingOff();
    void switchCharging();
    
    void activate() override;
    void deactivate() override;
};