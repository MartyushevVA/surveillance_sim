#include "objects.h"
#include "modules/modules.h"

#include <algorithm>
#include <cmath>
#include <limits>

void MobilePlatform::iterate(std::vector<Placeholder*> spottedSuspects) {
    if (auto sensor = findModuleOfType<SensorModule>()) {
        auto nearestSuspect = sensor->getNearestVisibleOpponent();
        if (WeaponModule* weapon = findModuleOfType<WeaponModule>())
            if (nearestSuspect)
                weapon->attack(nearestSuspect->getPosition());
        
        Pair nextPos;
        if (nearestSuspect)
            nextPos = opponentBasedMove(nearestSuspect->getPosition());
        else if (!spottedSuspects.empty())
            nextPos = opponentBasedMove(spottedSuspects[0]->getPosition());
        else
            nextPos = randomMove();
        
        if (auto connection = findModuleOfType<ConnectionModule>())
            if (connection->isSafeForSystem(nextPos))
                move(nextPos);
    }
}

void StaticPlatform::iterate(std::vector<Placeholder*>) {
    if (auto sensor = findModuleOfType<SensorModule>()) {
        auto nearestSuspect = sensor->getNearestVisibleOpponent();
        if (WeaponModule* weapon = findModuleOfType<WeaponModule>())
            if (nearestSuspect)
                weapon->attack(nearestSuspect->getPosition());
    }
}

void Suspect::iterate() {
    auto predator = getNearestVisibleOpponent();
    Pair nextPos;
    if (predator)
        nextPos = opponentBasedMove(predator->getPosition());
    else
        nextPos = randomMove();
    if (nextPos != getPosition())
        move(nextPos);
}

Report Suspect::getSurrounding() const {
    std::vector<std::shared_ptr<Placeholder>> tokensInRange;
    auto env = getEnvironment();
    auto position = getPosition();
    auto area = env->getArea(position, visionRange_);
    for (auto& [checkPos, token] : area)
        if (checkPos != getPosition())
            if (env->hasLineOfSight(position, checkPos))
                tokensInRange.push_back(token);
    return {position, tokensInRange};
}

std::shared_ptr<Placeholder> Suspect::getNearestVisibleOpponent() const {
    auto report = getSurrounding();
    std::shared_ptr<Placeholder> nearestPredator = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for (auto placeholder : report.objects)
        if (auto predator = dynamic_cast<Platform*>(placeholder.get())) {
            double distance = environment_->calculateDistance(position_, predator->getPosition());
            if (minDistance > distance) {
                minDistance = distance;
                nearestPredator = placeholder;
            }
        }
    return nearestPredator;
}