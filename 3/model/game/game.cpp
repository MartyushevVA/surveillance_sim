#include <thread>

#include "game.h"

#include "../objects/objects.h"
#include "../modules/modules.h"

void Game::initializeField(const GameConfig& config) {
    updateInterval_ = config.updateInterval;
    environment_.setSize(config.field.size.x, config.field.size.y);
    for (const auto& position : config.field.obstacles) {
        auto obstacle = std::make_shared<Obstacle>(position, &environment_);
        environment_.addToken(obstacle);
    }
    for (const auto& suspectConfig : config.field.suspects) {
        auto suspect = std::make_shared<Suspect>(
            suspectConfig.position,
            &environment_,
            suspectConfig.speed,
            suspectConfig.sensorRange
        );
        environment_.addToken(suspect);
    }
    for (const auto& platformConfig : config.field.platforms) {
        std::shared_ptr<Platform> platform;
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
                platformConfig.slotCount
            );
        }
        for (const auto& moduleConfig : platformConfig.modules) {
            std::shared_ptr<Module> module;
            if (moduleConfig.type == "ConnectionModule") {
                module = std::make_shared<ConnectionModule>(
                    moduleConfig.slotsOccupied,
                    moduleConfig.energyConsumption,
                    moduleConfig.isOn,
                    moduleConfig.range,
                    moduleConfig.specific.maxSessions
                );
            }
            else if (moduleConfig.type == "SensorModule") {
                module = std::make_shared<SensorModule>(
                    moduleConfig.slotsOccupied,
                    moduleConfig.energyConsumption,
                    moduleConfig.isOn,
                    moduleConfig.range,
                    moduleConfig.specific.sensorType
                );
            }
            else if (moduleConfig.type == "WeaponModule") {
                module = std::make_shared<WeaponModule>(
                    moduleConfig.slotsOccupied,
                    moduleConfig.energyConsumption,
                    moduleConfig.isOn,
                    moduleConfig.range,
                    moduleConfig.specific.chargingDuration
                );
            }
            platform->installModule(module);
        }
        environment_.addToken(platform);
        if (auto staticPlatform = dynamic_cast<StaticPlatform*>(platform.get())) {
            ai_.addStaticPlatform(staticPlatform);
        }
    }
}

void Game::start() {
    while (graphics_.isWindowOpen()) {
        auto start = std::chrono::steady_clock::now();
        graphics_.handleEvents();
        updateSuspects();
        ai_.eliminateAllSuspects();
        graphics_.render(environment_);
        auto end = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(updateInterval_ - (end - start));
    }
}

void Game::updateSuspects() {
    for (const auto& token : environment_.getTokens()) {
        if (Suspect* suspect = dynamic_cast<Suspect*>(token.second.get())) {
            if (Platform* predator = suspect->nearestPredatorWithinRange())
                token.second->move(suspect->calculateAvoidanceMove(predator->getPosition()));
            else
                token.second->move(suspect->calculateRandomMove());
        }
    }
}