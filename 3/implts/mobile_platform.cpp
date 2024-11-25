#include "mobile_platform.h"

#include "suspect.h"
#include "platform.h"
#include "environment.h"
#include "module_types.h"

Pair MobilePlatform::findPursuitableSuspect(Report report) const {
    for (const auto& token : report.objects)
        if (Suspect* suspect = dynamic_cast<Suspect*>(token.get()))
            if (environment_->hasLineOfSight(position_, suspect->getPosition()) && environment_->howFar(position_, suspect->getPosition(), speed_) <= 1)
                return suspect->getPosition();
    return {-1, 0};
}

void MobilePlatform::positionRelatedUpdate(Pair newPosition) {
    if (newPosition == position_) return;
    if (auto connectionModule = findModuleOfType<ConnectionModule>())
        connectionModule->positionRelatedUpdate(newPosition);
    if (auto sensorModule = findModuleOfType<SensorModule>())
        sensorModule->positionRelatedUpdate(newPosition);
    if (auto weaponModule = findModuleOfType<WeaponModule>())
        weaponModule->positionRelatedUpdate(newPosition);
}