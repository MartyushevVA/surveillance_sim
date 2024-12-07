#include "modules.h"

#include "../objects/objects.h"

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

bool WeaponModule::attachableTo(std::shared_ptr<Platform> host) const {
    if (!host)
        throw std::invalid_argument("Host is not set");
    return ((int)host->getModules().size() + slotsOccupied_ <= host->getSlotCount());
}

void WeaponModule::update() {
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