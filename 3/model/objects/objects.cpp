#include "objects.h"
#include "modules/modules.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <future>

#include <iostream>

void MobilePlatform::iterate() {
    auto connection = findModuleOfType<ConnectionModule>();
    if (!connection) return;
    
    auto env = getEnvironment().lock();
    if (!env) return;

    auto sensor = findModuleOfType<SensorModule>();
    if (!sensor) return;

    auto aiConnection = connection->getConnectedToAIDirectly().lock();
    if (!aiConnection) return;
    
    auto aiHost = aiConnection->getHost();
    if (!aiHost) return;

    auto ai = aiHost->getAI().lock();
    if (!ai) return;

    std::pair<Pair, std::shared_ptr<Suspect>> target = {{-1, -1}, nullptr};
    {
        auto detected = env->getEnemies<Suspect>(sensor->getSurrounding().objects);
        if (!detected.empty()) {
            ai->addSuspects(detected);
        }
        auto suspects = ai->getSuspects();
        if (!suspects.empty()) {
            target = env->getClosestOfType<Suspect>(getPosition(), suspects);
        }
    }

    std::future<bool> attackResult;
    if (target.second) {
        if (auto weapon = findModuleOfType<WeaponModule>()) {
            weapon->update();
            attackResult = std::async(std::launch::async, [&]() {
                return weapon->attack(target.first);
            });
        }
    }

    Pair nextPos = target.second ? opponentBasedMove(target.first) : randomMove();

    if (attackResult.valid() && attackResult.get()) {
        ai->removeSuspect(target.second);
        return;
    }

    if (connection->isSafeForSystem(nextPos)) {
        move(nextPos);
    }
    
    connection->update();
}

void StaticPlatform::iterate() {
    auto connection = findModuleOfType<ConnectionModule>();
    if (!connection) return;

    auto ai = getAI().lock();
    if (!ai) return;

    auto env = getEnvironment().lock();
    if (!env) return;

    auto sensor = findModuleOfType<SensorModule>();
    if (!sensor) return;
    
    std::pair<Pair, std::shared_ptr<Suspect>> target = {{-1, -1}, nullptr};
    {
        auto detected = env->getEnemies<Suspect>(sensor->getSurrounding().objects);
        if (!detected.empty()) {
            ai->addSuspects(detected);
        }
        auto suspects = ai->getSuspects();
        if (!suspects.empty()) {
            target = env->getClosestOfType<Suspect>(getPosition(), suspects);
        }
    }

    if (target.second) {
        if (auto weapon = findModuleOfType<WeaponModule>()) {
            weapon->update();
            if (weapon->attack(target.first)) {
                ai->removeSuspect(target.second);
                return;
            }
        }
    }
    
    connection->update();
}

void Suspect::iterate() {
    auto env = getEnvironment().lock();

    std::pair<Pair, std::shared_ptr<Platform>> predator = {{-1, -1}, nullptr};
    auto predators = env->getEnemies<Platform>(getSurrounding().objects);
    if (!predators.empty()) {
        predator = env->getClosestOfType<Platform>(getPosition(), predators);
    }

    Pair nextPos = randomMove();
    if (predator.second)
        nextPos = opponentBasedMove(predator.first);
    move(nextPos);
}

Report Suspect::getSurrounding() const {
    std::map<Pair, std::shared_ptr<Placeholder>> tokensInRange;
    auto env = getEnvironment().lock();
    auto position = getPosition();
    auto area = env->getArea(position, visionRange_);
    for (auto& [checkPos, token] : area)
        if (checkPos != getPosition() && (env->hasLineOfSight(position, checkPos)))
                tokensInRange[checkPos] = token;
    return {position, tokensInRange};
}