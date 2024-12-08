#include "objects.h"
#include "modules/modules.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

void MobilePlatform::iterate() {
    auto connection = findModuleOfType<ConnectionModule>();
    connection->update();
    if (auto sensor = findModuleOfType<SensorModule>()) {    
        if (!connection) throw std::runtime_error("wtf lol");

        auto staticPlatform = connection->getConnectedToAIDirectly();
        if (!staticPlatform) throw std::runtime_error("No static platform");

        Pair nextPos;
        std::cout << getPosition().x << " " << getPosition().y << std::endl;

        staticPlatform->getAI()->addSuspects(sensor->getSuspects());
        auto nearestSuspect = environment_->getClosestOfType<Suspect>(getPosition(), staticPlatform->getAI()->getSuspects());

        if (WeaponModule* weapon = findModuleOfType<WeaponModule>()) {
            if (nearestSuspect) {
                std::cout << nearestSuspect->getPosition().x << " " << nearestSuspect->getPosition().y << std::endl;
                if (weapon->attack(nearestSuspect->getPosition()))
                    staticPlatform->getAI()->removeSuspect(nearestSuspect);
            }
        }

        if (!staticPlatform->getAI()->getSuspects().empty()) {
            auto target = environment_->getClosestOfType<Suspect>(getPosition(), staticPlatform->getAI()->getSuspects());
            nextPos = opponentBasedMove(target->getPosition());
        }
        else
            nextPos = randomMove();
        
        if (connection->isSafeForSystem(nextPos)) {
            move(nextPos);
            connection->update();
        }
    }
}

void StaticPlatform::iterate() {
    auto connection = findModuleOfType<ConnectionModule>();
    connection->update();
    if (auto sensor = findModuleOfType<SensorModule>()) {
        getAI()->addSuspects(sensor->getSuspects());
        auto nearestSuspect = environment_->getClosestOfType<Suspect>(getPosition(), getAI()->getSuspects());
        if (WeaponModule* weapon = findModuleOfType<WeaponModule>())
            if (nearestSuspect)
                if (weapon->attack(nearestSuspect->getPosition()))
                    getAI()->removeSuspect(nearestSuspect);
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