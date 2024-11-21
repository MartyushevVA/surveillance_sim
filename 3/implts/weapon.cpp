#include "module_types.h"

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

Pair WeaponModule::findAttackableSuspect(Report report) const {
    for (const auto& token : report.objects) {
        if (Suspect* suspect = dynamic_cast<Suspect*>(token.get()))
            if (host_->getEnvironment()->hasLineOfSight(host_->getPosition(), suspect->getPosition()) && (host_->getEnvironment()->howFar(host_->getPosition(), suspect->getPosition(), range_) <= 1))
                return suspect->getPosition();
    }
    return {-1, 0};
}

void WeaponModule::attack(Pair suspect) {
    if (!host_ || !isOn_ || !host_->getEnvironment()->hasLineOfSight(host_->getPosition(), suspect)) {
        return;
    }
    if (isCharged_) {
        host_->getEnvironment()->extractToken(suspect);
        isCharged_ = false;
    }
}

void WeaponModule::attachTo(Platform* host) const {
    host->installModule(std::make_unique<WeaponModule>(*this));
}