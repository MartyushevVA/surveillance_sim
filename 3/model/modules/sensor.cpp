#include "modules.h"

#include <cmath>

#include "../objects/objects.h"

Report SensorModule::getSurrounding() const {
    std::map<Pair, std::shared_ptr<Placeholder>> tokensInRange;

    auto hostPtr = host_.lock();
    auto env = hostPtr->getEnvironment().lock();
    auto position = hostPtr->getPosition();

    auto area = env->getArea(position, range_);
    for (auto& [checkPos, token] : area)
        if (checkPos != hostPtr->getPosition() && (env->hasLineOfSight(position, checkPos) || type_ == SensorType::XRay))
            tokensInRange[checkPos] = token;
    return {position, tokensInRange};
}

void SensorModule::turnOn() {
    setIsOn(true);
    host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() + energyConsumption_);
}

void SensorModule::turnOff() {
    setIsOn(false);
    host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() - energyConsumption_);
}
