#include "weapon_module.h"
#include "platform.h"

void WeaponModule::attack(Pair intruder) {
    if (!host_ || !isOn_ || !isInRange(intruder)) { //нужно учитывать препятствия isAccessible
        return;
    }
    if (isCharged_) {
        host_->getEnvironment()->setCellType(intruder.x, intruder.y, CellType::Empty);
        isCharged_ = false;
    }
}

void WeaponModule::startCharging() {
    if (!host_ || !isOn_ || isCharging_ || isCharged_)
        return;
    if (host_->getMaxEnergyLevel() < host_->getEnergyLevel() + energyConsumption_)
        return;
    isCharging_ = true;
    chargingStarted_ = std::chrono::steady_clock::now();
    host_->setEnergyLevel(host_->getEnergyLevel() + energyConsumption_);
}

void WeaponModule::refreshState() {
    if (isCharged_) return;
    if (!isCharging_) startCharging();
    else {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - chargingStarted_);
        if (elapsedTime >= chargingDuration_) {
            isCharged_ = true;
            isCharging_ = false;
            host_->setEnergyLevel(host_->getEnergyLevel() - energyConsumption_);
        }
    }
}

void WeaponModule::attachTo(Platform* host) const {
    host->installModule(std::make_unique<WeaponModule>(*this));
}