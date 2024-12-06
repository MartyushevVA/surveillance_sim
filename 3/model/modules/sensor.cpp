#include "modules.h"

#include <cmath>

#include "../objects/objects.h"

Report SensorModule::getSurrounding() const {
    std::vector<std::shared_ptr<Placeholder>> tokensInRange;
    auto hostPtr = host_.lock();
    auto env = host_.lock()->getEnvironment();
    auto position = host_.lock()->getPosition();
    auto area = env->getArea(position, range_);
    for (auto& [checkPos, token] : area)
        if (checkPos != hostPtr->getPosition())
            if (type_ == SensorType::XRay || env->hasLineOfSight(position, checkPos))
                tokensInRange.push_back(token);
    return {position, tokensInRange};
}

bool SensorModule::attachableTo(std::shared_ptr<Platform> host) const {
    if (!host)
        throw std::invalid_argument("Host is not set");
    return (host->getEnergyLevel() + energyConsumption_ <= host->getMaxEnergyLevel())
    && ((int)host->getModules().size() + slotsOccupied_ <= host->getSlotCount());
}

std::shared_ptr<Placeholder> SensorModule::getVisibleSuspect(Report report) const {
    for (auto placeholder : report.objects)
        if (Suspect* suspect = dynamic_cast<Suspect*>(placeholder.get()))
            if (host_.lock()->getEnvironment()->hasLineOfSight(report.position, suspect->getPosition()))
                return placeholder;
    return nullptr;
}

void SensorModule::setUp() {
    if (host_.lock())
        host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() + energyConsumption_);
}