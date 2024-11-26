#include <iostream>

#include "module_types.h"

#include "platform.h"
#include "environment.h"

Report SensorModule::getSurrounding() const {
    std::vector<std::shared_ptr<Placeholder>> tokensInRange;
    if (!host_.lock())
        throw std::invalid_argument("Host is not set");
    
    auto environment = host_.lock()->getEnvironment();
    auto position = host_.lock()->getPosition();
    if (!environment)
        throw std::invalid_argument("Environment is not set");
    for (const auto& token : environment->getTokens())
        if ((environment->howFar(position, token->getPosition(), range_) <= 1) && token != host_.lock())
            if (type_ == SensorType::XRay || environment->hasLineOfSight(position, token->getPosition()))
                tokensInRange.push_back(token);
    return {position, tokensInRange};
}

bool SensorModule::attachableTo(std::shared_ptr<Platform> host) const {
    if (!host)
        throw std::invalid_argument("Host is not set");
    return (host->getEnergyLevel() + energyConsumption_ <= host->getMaxEnergyLevel())
    && (host->getModules().size() + slotsOccupied_ <= host->getSlotCount());
}

void SensorModule::refresh() {}

void SensorModule::positionRelatedUpdate(Pair newPosition) {}

void SensorModule::setUp() {}