#pragma once

#include "module.h"
#include <chrono>

class Intruder;

class WeaponModule : public Module {
private:
    std::chrono::milliseconds chargingDuration_;
public:
    WeaponModule() = default;
    explicit WeaponModule(std::chrono::milliseconds chargingDuration);
    ~WeaponModule() = default;
    
    void eliminateIntruder(Intruder& intruder);
    void chargingOn();
    void chargingOff();
    void switchCharging();
    
    void activate() override;
    void deactivate() override;
};