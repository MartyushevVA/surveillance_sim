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

void Game::suspectThread(std::shared_ptr<Suspect> suspect) {
    while (isRunning_) {
        Pair nextPosition;
        {
            std::lock_guard<std::mutex> lock(environmentMutex_);
            suspect->iterate();
        }
        {
            std::lock_guard<std::mutex> lock(environmentMutex_);
            suspect->move(nextPosition);
        }
        std::this_thread::sleep_for(updateInterval_);
    }
}

void Game::platformThread(std::shared_ptr<MobilePlatform> platform) {
    while (isRunning_) {
        {
            std::lock_guard<std::mutex> lock(environmentMutex_);
            platform->iterate(ai_.getSpottedSuspects());
        }
        std::this_thread::sleep_for(updateInterval_);
    }
}

void Game::updateEntitiesParallel() {
    std::vector<std::thread> threads;
    for (const auto& [pos, token] : environment_.getTokens()) {
        if (auto suspect = std::dynamic_pointer_cast<Suspect>(token)) {
            threads.emplace_back(&Game::suspectThread, this, suspect);
        }
        if (auto platform = std::dynamic_pointer_cast<MobilePlatform>(token)) {
            threads.emplace_back(&Game::platformThread, this, platform);
        }
    }
    for (auto& thread : threads) {
        thread.detach();
    }
}

void Game::start() {
    isRunning_ = true;
    updateEntitiesParallel();

    while (graphics_.isWindowOpen()) {
        auto start = std::chrono::steady_clock::now();
        graphics_.handleEvents();
        {
            std::lock_guard<std::mutex> lock(environmentMutex_);
            ai_.eliminateAllSuspects();
            graphics_.render(environment_);
        }
        auto end = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(updateInterval_ - (end - start));
    }
    isRunning_ = false;
}