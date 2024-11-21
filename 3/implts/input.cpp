#include <fstream>
#include <nlohmann/json.hpp>

#include "game.h"


using json = nlohmann::json;

std::unique_ptr<Module> loadModule(const json& moduleData) {
    std::string type = moduleData["type"];
    int slotsOccupied = moduleData["slotsOccupied"];
    int energyConsumption = moduleData["energyConsumption"];
    bool isOn = moduleData["isOn"];
    int range = moduleData["range"];
    if (type == "ConnectionModule") {
        int maxSessions = moduleData["maxSessions"];
        return std::make_unique<ConnectionModule>(slotsOccupied, energyConsumption, isOn, range, maxSessions);
    }
    else if (type == "SensorModule") {
        std::string sensorType = moduleData["sensorType"];
        SensorType typeEnum = (sensorType == "XRay") ? SensorType::XRay : SensorType::Optical;
        return std::make_unique<SensorModule>(slotsOccupied, energyConsumption, isOn, range, typeEnum);
    }
    else if (type == "WeaponModule") {
        std::string chargingDuration = moduleData["chargingDuration"];
        return std::make_unique<WeaponModule>(slotsOccupied, energyConsumption, isOn, range, chargingDuration);
    }
    else;
}

std::shared_ptr<Platform> loadPlatform(const json& platformData, Environment& environment) {
    std::string type = platformData["type"];
    Pair position = {platformData["position"]["x"], platformData["position"]["y"]};
    std::string description = platformData["description"];
    int maxEnergyLevel = platformData["maxEnergyLevel"];
    int slotCount = platformData["slotCount"];
    int speed = platformData["speed"];
    if (type == "MobilePlatform") {
        return std::make_shared<MobilePlatform>(position, &environment, description, maxEnergyLevel, slotCount, speed);
    }
    else if (type == "StaticPlatform") {
        return std::make_shared<StaticPlatform>(position, &environment, description, maxEnergyLevel, slotCount);
    }
    else;
}

void Game::loadFieldFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open())
        throw std::runtime_error("Could not open file");
    json j;
    inputFile >> j;
    Pair size = {j["field"]["width"], j["field"]["height"]};
    environment_.setSize(size.x, size.y); 

    for (const auto& obstacleData : j["obstacles"]) {
        Pair position = {obstacleData["position"]["x"], obstacleData["position"]["y"]};
        auto obstacle = std::make_shared<Obstacle>(position, &environment_);
        environment_.addToken(obstacle);
    }

    for (const auto& intruderData : j["intruders"]) {
        Pair position = {intruderData["position"]["x"], intruderData["position"]["y"]};
        auto intruder = std::make_shared<Intruder>(position, &environment_);
        environment_.addToken(intruder);
    }

    for (const auto& platformData : j["platforms"]) {
        auto platform = loadPlatform(platformData, environment_);
        environment_.addToken(platform);
        if (dynamic_cast<StaticPlatform*>(platform.get()))
            ai_.addConnectedPlatform(platform);
        if (platformData["modules"]) {
            for (const auto& moduleData : platformData["modules"]) {
                auto module = loadModule(moduleData);
                module->attachTo(platform.get());
            }
        }
    }

    for (const auto& moduleData : j["modules"]) {
        auto module = loadModule(moduleData);
        if (moduleData["host_position"]) {
            Pair hostPosition = {moduleData["host_position"]["x"], moduleData["host_position"]["y"]};
            auto host = dynamic_cast<Platform*>(environment_.getToken(hostPosition).get());
            if (host)
                module->attachTo(host);
        } 
        else addToStorage(std::move(module));
    }
}
