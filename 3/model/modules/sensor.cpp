#include "modules.h"

#include <cmath>

#include "../objects/objects.h"

Report SensorModule::getSurrounding() const {
    std::vector<std::shared_ptr<Placeholder>> tokensInRange;
    auto hostPtr = host_.lock();
    if (!hostPtr) return {{}, {}};
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

std::shared_ptr<Placeholder> SensorModule::getNearestVisibleOpponent() const {
    auto report = getSurrounding();
    std::shared_ptr<Placeholder> nearestThreat = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for (auto placeholder : report.objects)
        if (auto threat = dynamic_cast<Suspect*>(placeholder.get())) {
            if (host_.lock()->getEnvironment()->hasLineOfSight(report.position, threat->getPosition())) {
                double distance = host_.lock()->getEnvironment()->calculateDistance(report.position, threat->getPosition());
                if (minDistance > distance) {
                    minDistance = distance;
                    nearestThreat = placeholder;
                }
            }
        }
    return nearestThreat;
}

std::vector<std::shared_ptr<Placeholder>> SensorModule::getSuspects() const {
    auto report = getSurrounding();
    std::vector<std::shared_ptr<Placeholder>> suspects;
    for (auto placeholder : report.objects)
        if (dynamic_cast<Suspect*>(placeholder.get()))
            suspects.push_back(placeholder);
    return suspects;
}

void SensorModule::setUp() {
    if (host_.lock())
        host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() + energyConsumption_);
}