#include "modules.h"

#include "../objects/objects.h"

void WeaponModule::startCharging() {
    auto hostPtr = host_.lock();
    if (!hostPtr) return;   
    if (hostPtr->getEnergyLevel() + energyConsumption_ > hostPtr->getMaxEnergyLevel())
        return;
    hostPtr->setEnergyLevel(hostPtr->getEnergyLevel() + energyConsumption_);
    isCharging_ = true;
    chargingStarted_ = std::chrono::steady_clock::now();
}

void WeaponModule::stopCharging() {
    host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() - energyConsumption_);
    isCharging_ = false;
}

bool WeaponModule::attack(Pair suspect) {
    auto hostPtr = host_.lock();
    if (!hostPtr || !isOn_ || !hostPtr->getEnvironment()->hasLineOfSight(hostPtr->getPosition(), suspect) ||
    hostPtr->getEnvironment()->isInRange(hostPtr->getPosition(), suspect, range_) > 1)
        return false;
    update();
    if (isCharged_) {
        if (hostPtr->getEnvironment()->getToken(suspect) != nullptr) {
            hostPtr->getEnvironment()->removeToken(suspect);
            isCharged_ = false;
            startCharging();
            return true;
        }
    }
    return false;
}

void WeaponModule::update() {
    if (isOn_) {
        if (isCharged_) return;
        if (isCharging_) {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - chargingStarted_);
            if (elapsedTime >= chargingDuration_) {
                host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() - energyConsumption_);
                isCharged_ = true;
                isCharging_ = false;
            }
            return;
        }
        startCharging();
        return;
    }
    if (isCharging_)
        stopCharging();
}

void WeaponModule::turnOn() {
    setIsOn(true);
    update();
}

void WeaponModule::turnOff() {
    setIsOn(false);
    update();
}