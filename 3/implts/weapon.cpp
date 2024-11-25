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

Pair WeaponModule::findAttackableSuspect(Report report) const {
    for (const auto& token : report.objects) {
        if (Suspect* suspect = dynamic_cast<Suspect*>(token.get()))
            if (host_.lock()->getEnvironment()->hasLineOfSight(host_.lock()->getPosition(), suspect->getPosition()) && (host_.lock()->getEnvironment()->howFar(host_.lock()->getPosition(), suspect->getPosition(), range_) <= 1)) {
                //std::cout << "Suspect is a black male running at " << suspect->getPosition().x << "," << suspect->getPosition().y << std::endl;
                return suspect->getPosition();
            }
    }
    return {-1, 0};
}

void WeaponModule::attack(Pair suspect) {
    if (!host_.lock() || !isOn_ || !host_.lock()->getEnvironment()->hasLineOfSight(host_.lock()->getPosition(), suspect))
        return;
    if (isCharged_) {
        std::cout << "Shots fired at "<< suspect.x <<","<< suspect.y << std::endl;
        host_.lock()->getEnvironment()->removeToken(suspect);
        isCharged_ = false;
        startCharging();
    }
}

bool WeaponModule::attachableTo(std::shared_ptr<Platform> host) const {
    if (!host)
        throw std::invalid_argument("Host is not set");
    return host->getModules().size() + slotsOccupied_ <= host->getSlotCount();
}

void WeaponModule::refresh() {
    if (isCharged_) return;
    if (!isCharging_) {
        startCharging();
        return;
    }
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - chargingStarted_);
    if (elapsedTime >= chargingDuration_) {
        host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() - energyConsumption_);
        isCharged_ = true;
        isCharging_ = false;
    }
}

void WeaponModule::positionRelatedUpdate(Pair newPosition) {}

void WeaponModule::setUp() {}