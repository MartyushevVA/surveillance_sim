#include "module_types.h"
#include <iostream>


#include "platform.h"
#include "suspect.h"
#include "environment.h"

void WeaponModule::startCharging() {
    auto hostPtr = host_.lock();
    if (!hostPtr || !isOn_ || isCharging_ || isCharged_)
        return;   
    if (hostPtr->getEnergyLevel() + energyConsumption_ > hostPtr->getMaxEnergyLevel())
        return;
    hostPtr->setEnergyLevel(hostPtr->getEnergyLevel() + energyConsumption_);
    isCharging_ = true;
    chargingStarted_ = std::chrono::steady_clock::now();
}

bool WeaponModule::attack(Pair suspect) {
    if (!host_.lock() || !isOn_ || !host_.lock()->getEnvironment()->hasLineOfSight(host_.lock()->getPosition(), suspect) ||
    host_.lock()->getEnvironment()->howFar(host_.lock()->getPosition(), suspect, range_) > 1)
        return false;
    update();
    if (isCharged_) {
        std::cout << "Shots fired at "<< suspect.x <<","<< suspect.y << std::endl;
        host_.lock()->getEnvironment()->removeToken(suspect);
        isCharged_ = false;
        startCharging();
        return true;
    }

    return false;
}

bool WeaponModule::attachableTo(std::shared_ptr<Platform> host) const {
    if (!host)
        throw std::invalid_argument("Host is not set");
    return host->getModules().size() + slotsOccupied_ <= host->getSlotCount();
}

void WeaponModule::update() {
    if (isCharged_) return;
    if (!isCharging_) {
        std::cout << "Charging started at " << host_.lock()->getEnergyLevel() << std::endl;
        startCharging();
        return;
    }
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - chargingStarted_);
    if (elapsedTime >= chargingDuration_) {
        std::cout << "energy level: " << host_.lock()->getEnergyLevel() << std::endl;
        host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() - energyConsumption_);
        isCharged_ = true;
        isCharging_ = false;
    }
}

void WeaponModule::setUp() {}