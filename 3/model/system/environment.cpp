#include "environment.h"

#include <cmath>
#include <mutex>

#include "../objects/objects.h"

/*void Game::initializeField(const GameConfig& config) {
    updateInterval_ = config.updateInterval;
    environment_.setSize(config.field.size.x, config.field.size.y);
    std::cout << "Setting field size" << std::endl;
    for (const auto& position : config.field.obstacles) {
        auto obstacle = std::make_shared<Obstacle>(position, &environment_);
        environment_.addToken(obstacle);
    }
    for (const auto& suspectConfig : config.field.suspects) {
        std::cout << "Adding suspect to environment" << std::endl;
        auto suspect = std::make_shared<Suspect>(
            suspectConfig.position,
            &environment_,
            suspectConfig.speed,
            suspectConfig.sensorRange
        );
        std::cout << "Adding suspect to AI" << std::endl;
        environment_.addToken(suspect);
        std::cout << "Adding suspect to AI" << std::endl;
    }
    for (const auto& platformConfig : config.field.platforms) {
        std::shared_ptr<Platform> platform;
        std::cout << "Creating platform" << std::endl;
        if (platformConfig.type == "MobilePlatform") {
            platform = std::make_shared<MobilePlatform>(
                platformConfig.position,
                &environment_,
                platformConfig.description,
                platformConfig.maxEnergyLevel,
                platformConfig.slotCount,
                platformConfig.speed
            );
        } else {
            platform = std::make_shared<StaticPlatform>(
                platformConfig.position,
                &environment_,
                platformConfig.description,
                platformConfig.maxEnergyLevel,
                platformConfig.slotCount,
                &ai_
            );
        }
        for (const auto& moduleConfig : platformConfig.modules) {
            std::shared_ptr<Module> module;
            if (moduleConfig.type == "ConnectionModule") {
                module = std::make_shared<ConnectionModule>(
                    moduleConfig.slotsOccupied,
                    moduleConfig.energyConsumption,
                    moduleConfig.range,
                    moduleConfig.specific.maxSessions
                );
            }
            else if (moduleConfig.type == "SensorModule") {
                module = std::make_shared<SensorModule>(
                    moduleConfig.slotsOccupied,
                    moduleConfig.energyConsumption,
                    moduleConfig.range,
                    moduleConfig.specific.sensorType
                );
            }
            else if (moduleConfig.type == "WeaponModule") {
                module = std::make_shared<WeaponModule>(
                    moduleConfig.slotsOccupied,
                    moduleConfig.energyConsumption,
                    moduleConfig.range,
                    moduleConfig.specific.chargingDuration
                );
            }
            std::cout << "Installing module" << std::endl;
            platform->installModule(module);

        }
        std::cout << "Adding platform to environment" << std::endl;
        environment_.addToken(std::dynamic_pointer_cast<Placeholder>(platform));
        std::cout << "Adding platform to AI" << std::endl;
        ai_.addStaticPlatform(std::dynamic_pointer_cast<StaticPlatform>(platform));
    }
}*/




void Environment::addToken(std::shared_ptr<Placeholder> token) {
    if (!token)
        return;
    std::cout << "Adding token to envddddaironment" << std::endl;
    if (token->getPosition().x >= size_.x || token->getPosition().y >= size_.y ||
        token->getPosition().x < 0 || token->getPosition().y < 0)
        throw std::invalid_argument("Token position is out of bounds");
    std::cout << "Adddddding token to envddddaironment" << std::endl;
    std::cout << "Token position: " << token->getPosition().x << ", " << token->getPosition().y << std::endl;
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
    std::cout<< "gettin token"<<std::endl;
    std::shared_lock<std::shared_mutex> lock(mutexes_[position.x + position.y * size_.x]);
    std::cout<< "gettin tddddoken"<<std::endl;
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
    std::cout << "Checking if position is empty" << std::endl;
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;
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
    std::shared_lock<std::shared_mutex> lock(environmentMutex_);
    std::vector<std::shared_lock<std::shared_mutex>> locks;
    for (auto pos : line)
        locks.emplace_back(mutexes_[pos.x + pos.y * size_.x]);
    //lock.unlock();
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
    std::vector<Pair> area;
    for (int dx = -range; dx <= range; dx++)
        for (int dy = -sqrt(range * range - dx * dx); dy <= sqrt(range * range - dx * dx); dy++) {
            Pair checkPos{position.x + dx, position.y + dy};
            if (checkPos.x >= size_.x || checkPos.y >= size_.y)
                continue;
            area.push_back(checkPos);
        }
    return area;
}

std::map<Pair, std::shared_ptr<Placeholder>> Environment::getArea(Pair position, int range) const {
    std::map<Pair, std::shared_ptr<Placeholder>> area;
    std::shared_lock<std::shared_mutex> lock(environmentMutex_);
    std::vector<std::shared_lock<std::shared_mutex>> locks;
    auto rArea = representArea(position, range);
    for (auto pos : rArea)
        locks.emplace_back(mutexes_[pos.x + pos.y * size_.x]);
    lock.unlock();
    for (auto pos : rArea) {
        if (pos == position || isEmpty(pos))
            continue;
        area[pos] = getToken(pos);
    }
    return area;
}