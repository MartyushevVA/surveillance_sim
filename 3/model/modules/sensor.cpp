#include "modules.h"

#include <cmath>

#include "../objects/objects.h"

Report SensorModule::getSurrounding() const {
    std::map<Pair, std::shared_ptr<Placeholder>> tokensInRange;
    auto hostPtr = host_.lock();
    if (!hostPtr) return {{}, {}};
    auto env = hostPtr->getEnvironment().lock();
    auto position = hostPtr->getPosition();
    auto area = env->getArea(position, range_);
    for (auto& [checkPos, token] : area)
        if (checkPos != hostPtr->getPosition())
            if (type_ == SensorType::XRay || env->hasLineOfSight(position, checkPos))
                tokensInRange[checkPos] = token;
    return {position, tokensInRange};
}

std::shared_ptr<Placeholder> SensorModule::getNearestVisibleOpponent() const {
    auto report = getSurrounding();
    std::shared_ptr<Placeholder> nearestThreat = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for (auto [pos, placeholder] : report.objects)
        if (dynamic_cast<Suspect*>(placeholder.get())) {
            if (host_.lock()->getEnvironment().lock()->hasLineOfSight(report.position, pos)) {
                double distance = host_.lock()->getEnvironment().lock()->calculateDistance(report.position, pos);
                if (minDistance > distance) {
                    minDistance = distance;
                    nearestThreat = placeholder;
                }
            }
        }
    return nearestThreat;
}

std::map<Pair, std::shared_ptr<Suspect>> SensorModule::getSuspects() const {
    auto report = getSurrounding();
    std::map<Pair, std::shared_ptr<Suspect>> suspects;
    for (auto [pos, placeholder] : report.objects)
        if (auto suspect = std::dynamic_pointer_cast<Suspect>(placeholder))
            suspects[pos] = suspect;
    return suspects;
}

void SensorModule::turnOn() {
    setIsOn(true);
    host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() + energyConsumption_);
}

void SensorModule::turnOff() {
    setIsOn(false);
    host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() - energyConsumption_);
}
