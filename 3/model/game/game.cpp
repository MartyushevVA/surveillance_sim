#include "game.h"

#include "../objects/objects.h"
#include "../modules/modules.h"
#include "../system/ai.h"

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
            platform->installModule(module);

        }
        environment_.addToken(std::dynamic_pointer_cast<Placeholder>(platform));
        ai_.addStaticPlatform(std::dynamic_pointer_cast<StaticPlatform>(platform));
    }
}

void Game::start() {
    isRunning_ = true;

    auto lastUpdate = std::chrono::steady_clock::now();
    while (graphics_.isWindowOpen()) {
        auto currentTime = std::chrono::steady_clock::now();
        auto deltaTime = currentTime - lastUpdate;
        graphics_.handleEvents();
        {
            for (const auto& [pos, token] : environment_.getTokens())
                if (auto suspect = std::dynamic_pointer_cast<Suspect>(token))
                    suspect->iterate();
            ai_.eliminateAllSuspects();
            graphics_.render(environment_);
        }
        lastUpdate = currentTime;
        if (deltaTime < updateInterval_)
            std::this_thread::sleep_for(updateInterval_ - deltaTime);
    }
    isRunning_ = false;
}