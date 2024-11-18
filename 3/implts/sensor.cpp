#include "module_types.h"

std::vector<std::shared_ptr<Placeholder>> SensorModule::getSurrounding() const {
    std::vector<std::shared_ptr<Placeholder>> tokensInRange;
    for (const auto& token : host_->getEnvironment()->getTokens()) {
        int x = token->getPosition().x;
        int y = token->getPosition().y;
        if ((x - host_->getPosition().x) * (x - host_->getPosition().x) + (y - host_->getPosition().y) * (y - host_->getPosition().y) <= getRange() * getRange())
            if (type_ == SensorType::XRay || host_->getEnvironment()->hasLineOfSight(host_->getPosition(), token->getPosition()))
                tokensInRange.push_back(token);
    }
    return tokensInRange;
}

void SensorModule::attachTo(Platform* host) const {
    if (host->getEnergyLevel() + energyConsumption_ <= host->getMaxEnergyLevel())
        host->installModule(std::make_unique<SensorModule>(*this));
}