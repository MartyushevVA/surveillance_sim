#include <iostream>

#include "module_types.h"

#include "platform.h"
#include "environment.h"
#include "suspect.h"

Report SensorModule::getSurrounding() const {
    std::vector<std::shared_ptr<Placeholder>> tokensInRange;
    auto hostPtr = host_.lock();
    auto env = host_.lock()->getEnvironment();
    auto position = host_.lock()->getPosition();
    for (int dx = -range_; dx <= range_; dx++)
        for (int dy = -sqrt(range_ * range_ - dx * dx); dy <= sqrt(range_ * range_ - dx * dx); dy++) {
            Pair checkPos{position.x + dx, position.y + dy};
            if (checkPos.x < 0 || checkPos.y < 0 || 
                checkPos.x >= env->getSize().x || 
                checkPos.y >= env->getSize().y)
                continue;
            if ((env->howFar(position, checkPos, range_) <= 1) && checkPos != hostPtr->getPosition())
                if (type_ == SensorType::XRay || env->hasLineOfSight(position, checkPos))
                    tokensInRange.push_back(env->getToken(checkPos));
        }
    return {position, tokensInRange};
}

bool SensorModule::attachableTo(std::shared_ptr<Platform> host) const {
    if (!host)
        throw std::invalid_argument("Host is not set");
    return (host->getEnergyLevel() + energyConsumption_ <= host->getMaxEnergyLevel())
    && (host->getModules().size() + slotsOccupied_ <= host->getSlotCount());
}
std::shared_ptr<Placeholder> SensorModule::getVisibleSuspect(Report report) const {
    for (auto placeholder : report.objects)
        if (Suspect* suspect = dynamic_cast<Suspect*>(placeholder.get()))
            if (host_.lock()->getEnvironment()->hasLineOfSight(report.position, suspect->getPosition()))
                return placeholder;
    return nullptr;
}
void SensorModule::update() {}

void SensorModule::setUp() {
    if (host_.lock())
        host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() + energyConsumption_);
}