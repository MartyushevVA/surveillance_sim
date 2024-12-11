#include "environment.h"

#include <cmath>
#include <mutex>

#include "../objects/objects.h"
#include "../modules/modules.h"

void Environment::initialize(SystemConfig config) {
    if (initialized_)
        throw std::runtime_error("Environment is already initialized");
    initialized_ = true;
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
    checkInitialized();
    if (!token)
        return;
    if (token->getPosition().x >= size_.x || token->getPosition().y >= size_.y ||
        token->getPosition().x < 0 || token->getPosition().y < 0)
        throw std::invalid_argument("Token position is out of bounds");
    if (isEmpty(token->getPosition()))
        tokens_.insert({token->getPosition(), token});
}


void Environment::removeToken(Pair position) {
    checkInitialized();
    auto token = getToken(position);
    if (!token) return;
    std::unique_lock<std::shared_mutex> lock(token->getMutex());
    tokens_.erase(position);
}

std::shared_ptr<Placeholder> Environment::getToken(Pair position) const {
    checkInitialized();
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto token = tokens_.find(position);
    if (token != tokens_.end()) {
        return token->second;
    }
    return nullptr;
}

bool Environment::abilityToMove(Pair from, Pair to) const {
    checkInitialized();
    if (to.x >= size_.x || to.y >= size_.y || to.x < 0 || to.y < 0)
        return false;
    if (!hasLineOfSight(from, to))
        return false;
    return true;
}

void Environment::moveToken(Pair from, Pair to) {
    checkInitialized();
    auto token = getToken(from);
    if (to.x >= size_.x || to.y >= size_.y || to.x < 0 || to.y < 0 || from == to)
        return;
    if (token && isEmpty(to)) {
        tokens_.erase(from);
        token->setPosition(to);
        tokens_.insert({to, token});
    }
}

bool Environment::isEmpty(Pair position) const {
    checkInitialized();
    return getToken(position) == nullptr;
}

std::vector<Pair> Environment::getLine(Pair from, Pair to) const {
    checkInitialized();
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
    checkInitialized();
    auto line = getLine(from, to);
    for (auto pos : line) {
        
        auto token = getToken(pos);
        if (token && !isEmpty(token->getPosition())) {
            return false;
        }
    }
    return true;
}

double Environment::isInRange(Pair from, Pair to, int range) const {
    checkInitialized();
    return calculateDistance(from, to) / range;
}

double Environment::calculateDistance(Pair from, Pair to) const {
    checkInitialized();
    return sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
}

std::vector<Pair> Environment::representArea(Pair position, int range) const {
    checkInitialized();
    std::vector<Pair> area;
    for (int dx = -range; dx <= range; dx++)
        for (int dy = -sqrt(range * range - dx * dx); dy <= sqrt(range * range - dx * dx); dy++) {
            Pair checkPos{position.x + dx, position.y + dy};
            if (checkPos.x >= size_.x || checkPos.y >= size_.y || checkPos.x < 0 || checkPos.y < 0)
                continue;
            area.push_back(checkPos);
        }
    return area;
}

std::map<Pair, std::shared_ptr<Placeholder>> Environment::getArea(Pair position, int range) const {
    std::map<Pair, std::shared_ptr<Placeholder>> area;
    auto rArea = representArea(position, range);
    //std::shared_lock<std::shared_mutex> lock(mutex_);
    for (auto pos : rArea) {
        if (pos == position || isEmpty(pos))
            continue;
        area[pos] = getToken(pos);
    }
    return area;
}