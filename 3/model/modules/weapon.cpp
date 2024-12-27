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

void WeaponModule::pause() {
    if (isCharging_) {
        auto currentTime = std::chrono::steady_clock::now();
        elapsedChargingTime_ += std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - chargingStarted_);
        isCharging_ = false;
        wasChargingBeforePause_ = true;
    }
}

void WeaponModule::resume() {
    if (wasChargingBeforePause_) {
        chargingStarted_ = std::chrono::steady_clock::now() - elapsedChargingTime_;
        isCharging_ = true;
        wasChargingBeforePause_ = false;
    }
}

bool WeaponModule::attack(Pair target) {
    if (!isCharged_)
        return false;
    auto platform = host_.lock();
    if (!platform) return false;
    auto env = platform->getEnvironment().lock();
    if (!env) return false;
    
    if (env->isInRange(platform->getPosition(), target, range_) <= 1.0 &&
        env->hasLineOfSight(platform->getPosition(), target)) {
        if (env->removeToken(target)) {
            isCharged_ = false;
            return true;
        }
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
            elapsedChargingTime_ = std::chrono::milliseconds(0);
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