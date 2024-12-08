#include "objects.h"
#include "modules/modules.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

void MobilePlatform::iterate() {
    auto connection = findModuleOfType<ConnectionModule>();
    if (!connection) return;
    
    connection->update();

    auto staticPlatform = connection->getConnectedToAIDirectly();
    if (!staticPlatform) return;
    
    auto ai = staticPlatform->getAI();
    if (!ai) return;
    
    std::shared_ptr<Suspect> target = nullptr;
    {
        auto suspects = ai->getSuspects();
        if (!suspects.empty())
            target = environment_->getClosestOfType<Suspect>(getPosition(), suspects);
    }
    
    auto nextPos = randomMove();
    if (target) {
        if (auto weapon = findModuleOfType<WeaponModule>()) {
            weapon->update();
            if (weapon->attack(target->getPosition())) {
                ai->removeSuspect(target);
                return;
            }
        }
        nextPos = opponentBasedMove(target->getPosition());
        
    }
    if (connection->isSafeForSystem(nextPos)) {
        move(nextPos);
        connection->update();
    }
}

void StaticPlatform::iterate() {
    auto connection = findModuleOfType<ConnectionModule>();
    if (!connection) return;
    connection->update();
    
    auto sensor = findModuleOfType<SensorModule>();
    if (!sensor) return;
    
    auto ai = getAI();
    if (!ai) return;
    
    ai->addSuspects(sensor->getSuspects());
    
    std::shared_ptr<Suspect> target = nullptr;
    {
        auto suspects = ai->getSuspects();
        if (!suspects.empty())
            target = environment_->getClosestOfType<Suspect>(getPosition(), suspects);
    }
    if (target)
        if (auto weapon = findModuleOfType<WeaponModule>()) {
            weapon->update();
            if (weapon->attack(target->getPosition())) {
                ai->removeSuspect(target);
                return;
            }
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
    std::map<Pair, std::shared_ptr<Placeholder>> tokensInRange;
    auto env = getEnvironment();
    auto position = getPosition();
    auto area = env->getArea(position, visionRange_);
    for (auto& [checkPos, token] : area)
        if (checkPos != getPosition())
            if (env->hasLineOfSight(position, checkPos))
                tokensInRange[checkPos] = token;
    return {position, tokensInRange};
}

std::shared_ptr<Placeholder> Suspect::getNearestVisibleOpponent() const {
    auto report = getSurrounding();
    std::shared_ptr<Placeholder> nearestPredator = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for (auto [pos, placeholder] : report.objects)
        if (dynamic_cast<Platform*>(placeholder.get())) {
            double distance = environment_->calculateDistance(position_, pos);
            if (minDistance > distance) {
                minDistance = distance;
                nearestPredator = placeholder;
            }
        }
    return nearestPredator;
}