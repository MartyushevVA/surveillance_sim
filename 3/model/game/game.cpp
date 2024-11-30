#include "game.h"

#include "../objects/objects.h"

void Game::initializeField(const FieldConfig& config) {
    environment_.setSize(config.size.width, config.size.height);
    for (const auto& position : config.obstacles) {
        auto obstacle = std::make_shared<Obstacle>(position, &environment_);
        environment_.addToken(obstacle);
    }
    for (const auto& suspectConfig : config.suspects) {
        auto suspect = std::make_shared<Suspect>(
            suspectConfig.position,
            &environment_,
            suspectConfig.speed,
            suspectConfig.sensorRange
        );
        environment_.addToken(suspect);
    }
    for (const auto& platformConfig : config.platforms) {
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
        graphics_.handleEvents();
        updateSuspects();
        ai_.eliminateAllSuspects();
        graphics_.render(environment_);
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