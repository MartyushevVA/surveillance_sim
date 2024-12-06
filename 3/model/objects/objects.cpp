#include "objects.h"
#include "modules/modules.h"

#include <algorithm>
#include <cmath>
#include <limits>

void MobilePlatform::iterate(std::vector<Placeholder*> spottedSuspects) {
    if (auto sensor = findModuleOfType<SensorModule>()) {
        Report report = sensor->getSurrounding();
        if (WeaponModule* weapon = findModuleOfType<WeaponModule>())
            if (auto attackableSuspect = sensor->getVisibleSuspect(report))
                weapon->attack(attackableSuspect->getPosition());
        Pair nextPos;
        if (auto attackableSuspect = sensor->getVisibleSuspect(report))
            nextPos = opponentBasedMove(attackableSuspect->getPosition());
        else if (!spottedSuspects.empty())
            nextPos = opponentBasedMove(spottedSuspects[0]->getPosition());
        else
            nextPos = randomMove();
        if (ConnectionModule* connection = findModuleOfType<ConnectionModule>(); connection->isSafeForSystem(nextPos))
            move(nextPos);
    }
}

void Suspect::iterate() {
    if (Platform* predator = nearestPredatorWithinRange())
        move(opponentBasedMove(predator->getPosition()));
    else
        move(randomMove());
}

Platform* Suspect::nearestPredatorWithinRange() const {
    auto area = environment_->getArea(position_, sensorRange_);
    Platform* nearestPredator = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for (auto& [checkPos, token] : area)
        if (Platform* predator = dynamic_cast<Platform*>(token.get()))
            if (environment_->hasLineOfSight(position_, checkPos)) {
                double distance = environment_->howFar(position_, checkPos, sensorRange_);
                if (minDistance > distance) {
                    minDistance = distance;
                    nearestPredator = predator;
                }
            }
    return nearestPredator;
}