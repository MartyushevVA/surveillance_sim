#include "environment.h"

#include <cmath>
#include <mutex>

#include <iostream>

#include "../objects/objects.h"
#include "../modules/modules.h"

Environment::Environment(SystemConfig config) : size_{config.size}, mutexes_(config.size.x * config.size.y) {
    for (const auto& position : config.obstacles) {
        auto obstacle = std::make_shared<Obstacle>(position.position, weak_from_this());
        addToken(obstacle);
    }
    for (const auto& suspectConfig : config.suspects) {
        auto suspect = std::make_shared<Suspect>(
            suspectConfig.position,
            weak_from_this(),
            suspectConfig.speed,
            suspectConfig.sensorRange
        );
        addToken(suspect);
    }
    for (const auto& platformConfig : config.platforms) {
        std::shared_ptr<Platform> platform;
        if (platformConfig.type == PlatformType::MOBILE) {
            platform = std::make_shared<MobilePlatform>(
                platformConfig.position,
                weak_from_this(),
                platformConfig.description,
                platformConfig.maxEnergyLevel,
                platformConfig.slotCount,
                platformConfig.speed
            );
        } else {
            platform = std::make_shared<StaticPlatform>(
                platformConfig.position,
                weak_from_this(),
                platformConfig.description,
                platformConfig.maxEnergyLevel,
                platformConfig.slotCount
            );
        }
        for (const auto& moduleConfig : platformConfig.modules) {
            std::shared_ptr<Module> module;
            if (moduleConfig.type == ModuleType::CONNECTION) {
                module = std::make_shared<ConnectionModule>(
                    moduleConfig.slotsOccupied,
                    moduleConfig.energyConsumption,
                    moduleConfig.range,
                    moduleConfig.specific.maxSessions
                );
            }
            else if (moduleConfig.type == ModuleType::SENSOR) {
                module = std::make_shared<SensorModule>(
                    moduleConfig.slotsOccupied,
                    moduleConfig.energyConsumption,
                    moduleConfig.range,
                    moduleConfig.specific.sensorType
                );
            }
            else if (moduleConfig.type == ModuleType::WEAPON) {
                module = std::make_shared<WeaponModule>(
                    moduleConfig.slotsOccupied,
                    moduleConfig.energyConsumption,
                    moduleConfig.range,
                    moduleConfig.specific.chargingDuration
                );
            }
            platform->installModule(module);

        }
        addToken(std::dynamic_pointer_cast<Placeholder>(platform));
    }
}

void Environment::addToken(std::shared_ptr<Placeholder> token) {
    if (!token)
        return;
    if (token->getPosition().x >= size_.x || token->getPosition().y >= size_.y ||
        token->getPosition().x < 0 || token->getPosition().y < 0)
        throw std::invalid_argument("Token position is out of bounds");
    if (isEmpty(token->getPosition()))
        tokens_.insert({token->getPosition(), token});
}


void Environment::removeToken(Pair position) {
    std::unique_lock<std::shared_mutex> lock(mutexes_[position.x + position.y * size_.x]);
    if (isEmpty(position))
        return;
    tokens_.erase(position);
}

std::shared_ptr<Placeholder> Environment::getToken(Pair position) const {
    std::shared_lock<std::shared_mutex> lock(mutexes_[position.x + position.y * size_.x]);
    auto token = tokens_.find(position);
    return token != tokens_.end() ? token->second : nullptr;
}

bool Environment::abilityToMove(Pair from, Pair to) const {
    if (to.x >= size_.x || to.y >= size_.y || to.x < 0 || to.y < 0)
        return false;
    if (!hasLineOfSight(from, to))
        return false;
    return true;
}

void Environment::moveToken(Pair from, Pair to) {
    if (to.x >= size_.x || to.y >= size_.y || to.x < 0 || to.y < 0 || from == to)
        return;
    
    auto token = getToken(from);
    if (token && isEmpty(to)) {
        std::unique_lock<std::shared_mutex> lock1(mutexes_[from.x + from.y * size_.x]);
        std::unique_lock<std::shared_mutex> lock2(mutexes_[to.x + to.y * size_.x]);
        tokens_.erase(from);
        tokens_.insert({to, token});
        token->setPosition(to);
    }
}

bool Environment::isEmpty(Pair position) const {
    return getToken(position) == nullptr;
}

std::vector<Pair> Environment::getLine(Pair from, Pair to) const {
    std::vector<Pair> line;
    double distance = calculateDistance(from, to);
    double stepX = (to.x - from.x) / distance;
    double stepY = (to.y - from.y) / distance;
    for (int i = 0; i <= distance; i++) {
        int x = static_cast<int>(from.x + stepX * i);
        int y = static_cast<int>(from.y + stepY * i);
        if (Pair{x, y} == from || Pair{x, y} == to)
            continue;
        line.push_back(Pair{x, y});
    }
    return line;    
}

bool Environment::hasLineOfSight(Pair from, Pair to) const {
    auto line = getLine(from, to);
    std::vector<std::shared_lock<std::shared_mutex>> locks;
    for (auto pos : line)
        locks.emplace_back(mutexes_[pos.x + pos.y * size_.x]);
    for (auto pos : line) {
        if (!isEmpty(pos))
            return false;
    }
    return true;
}

double Environment::isInRange(Pair from, Pair to, int range) const {
    return calculateDistance(from, to) / range;
}

double Environment::calculateDistance(Pair from, Pair to) const {
    return sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
}

std::vector<Pair> Environment::representArea(Pair position, int range) const {
    std::cout << "Environment::representArea position at " << position.x << ", " << position.y << std::endl;
    std::vector<Pair> area;
    for (int dx = -range; dx <= range; dx++)
        for (int dy = -sqrt(range * range - dx * dx); dy <= sqrt(range * range - dx * dx); dy++) {
            std::cout << "Environment::representArea dx: " << dx << ", dy: " << dy << std::endl;
            Pair checkPos{position.x + dx, position.y + dy};
            if (checkPos.x >= size_.x || checkPos.y >= size_.y)
                continue;
            area.push_back(checkPos);
        }
    return area;
}

std::map<Pair, std::shared_ptr<Placeholder>> Environment::getArea(Pair position, int range) const {
    std::cout << "Environment::getArea position at " << position.x << ", " << position.y << std::endl;
    std::map<Pair, std::shared_ptr<Placeholder>> area;
    std::cout << "Environment::getArea before lock" << std::endl;
    std::vector<std::shared_lock<std::shared_mutex>> locks;
    std::cout << "Environment::getArea after lock" << std::endl;
    auto rArea = representArea(position, range);
    std::cout << "Environment::getArea rArea size: " << rArea.size() << std::endl;
    for (auto pos : rArea)
        locks.emplace_back(mutexes_[pos.x + pos.y * size_.x]);
    for (auto pos : rArea) {
        if (pos == position || isEmpty(pos))
            continue;
        area[pos] = getToken(pos);
    }
    return area;
}