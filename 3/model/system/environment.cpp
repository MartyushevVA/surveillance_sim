#include "environment.h"

#include <cmath>
#include <mutex>
#include <thread>
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
    if (!token)
        return;
    if (token->getPosition().x >= size_.x || token->getPosition().y >= size_.y ||
        token->getPosition().x < 0 || token->getPosition().y < 0)
        throw std::invalid_argument("Token position is out of bounds");
    if (isEmpty(token->getPosition()))
        tokens_.insert({token->getPosition(), token});
}


bool Environment::removeToken(Pair position) {
    auto token = getToken(position);
    if (!token) return false;
    tokens_.erase(token->getPosition());
    return true;
}

std::shared_ptr<Placeholder> Environment::getToken(Pair position) const {
    auto token = tokens_.find(position);
    if (token != tokens_.end())
        return token->second;
    return nullptr;
}

bool Environment::abilityToMove(Pair from, Pair to) const {
    if (!hasLineOfSight(from, to))
        return false;
    if (!isEmpty(to) || isEmpty(from))
        return false;
    return true;
}

void Environment::moveToken(Pair from, Pair to) {
    if (!abilityToMove(from, to))
        return;
    
    auto token = getToken(from);
    if (!token) return;
    tokens_.erase(token->getPosition());
    tokens_[to] = token;
    token->setPosition(to);
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
    return std::none_of(line.begin(), line.end(), 
        [this](const Pair& pos) { return tokens_.find(pos) != tokens_.end(); });
}

double Environment::isInRange(Pair from, Pair to, int range) const {
    return calculateDistance(from, to) / range;
}

double Environment::calculateDistance(Pair from, Pair to) const {
    return sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
}

std::vector<Pair> Environment::getAreaCords(Pair position, int range) const {
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
    auto coords = getAreaCords(position, range);
    std::map<Pair, std::shared_ptr<Placeholder>> area;
    
    for (auto pos : coords) {
        auto token = getToken(pos);
        if (token)
            area[pos] = token;
    }
    return area;
}