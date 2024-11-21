#include "module_types.h"

Report SensorModule::getSurrounding() const {
    std::vector<std::shared_ptr<Placeholder>> tokensInRange;
    for (const auto& token : host_->getEnvironment()->getTokens()) {
        if (host_->getEnvironment()->howFar(host_->getPosition(), token->getPosition(), range_) <= 1)
            if (type_ == SensorType::XRay || host_->getEnvironment()->hasLineOfSight(host_->getPosition(), token->getPosition()))
            tokensInRange.push_back(token);
    }
    return {host_->getPosition(), tokensInRange};
}

void SensorModule::attachTo(Platform* host) const {
    if (host->getEnergyLevel() + energyConsumption_ <= host->getMaxEnergyLevel())
        host->installModule(std::make_unique<SensorModule>(*this));
}