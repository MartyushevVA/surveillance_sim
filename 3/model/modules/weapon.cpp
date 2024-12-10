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

bool WeaponModule::attack(Pair target) {
    if (!isCharged_)
        return false;
    auto host = host_.lock();
    if (!host) return false;
    auto env = host->getEnvironment().lock();

    auto targetToken = env->getToken(target);
    if (!targetToken) return false;
    
    if (env->isInRange(host->getPosition(), target, range_) <= 1.0 &&
        env->hasLineOfSight(host->getPosition(), target)) {
        isCharged_ = false;
        env->removeToken(target);
        return true;
    }
    return false;
}

void WeaponModule::update() {
    if (!isOn_) {
        if (isCharging_)
            stopCharging();
        return;
    }
    if (isCharged_) return;
    if (isCharging_) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - chargingStarted_);
        
        if (elapsedTime >= chargingDuration_) {
            isCharged_ = true;
            isCharging_ = false;
        }
        return;
    }

    startCharging();
}

void WeaponModule::turnOn() {
    setIsOn(true);
    update();
}

void WeaponModule::turnOff() {
    setIsOn(false);
    update();
}