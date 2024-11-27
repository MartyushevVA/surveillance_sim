#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "module_types.h"
#include "game.h"

#include "mobile_platform.h"
#include "static_platform.h"
#include "suspect.h"


std::chrono::milliseconds parseChargingDuration(const std::string& durationStr) {
    int hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
    std::sscanf(durationStr.c_str(), "%d:%d:%d:%d", &hours, &minutes, &seconds, &milliseconds);
    return std::chrono::milliseconds(milliseconds + seconds * 1000 + minutes * 60000 + hours * 3600000);
}

using json = nlohmann::json;

std::shared_ptr<Module> loadModule(const json& moduleData) {
    const std::vector<std::string> requiredFields = {
        "type", "slotsOccupied", "energyConsumption", "isOn", "range"
    };
    for (const auto& field : requiredFields) {
        if (!moduleData.contains(field))
            throw std::runtime_error("Missing required module field: " + field);
    }

    std::string type = moduleData["type"].get<std::string>();
    int slotsOccupied = moduleData["slotsOccupied"].get<int>();
    int energyConsumption = moduleData["energyConsumption"].get<int>();
    bool isOn = moduleData["isOn"].get<bool>();
    int range = moduleData["range"].get<int>();
    if (type == "ConnectionModule") {
        int maxSessions = moduleData["maxSessions"].get<int>();
        return std::make_shared<ConnectionModule>(slotsOccupied, energyConsumption, isOn, range, maxSessions);
    }
    else if (type == "SensorModule") {
        std::string sensorType = moduleData["sensorType"].get<std::string>();
        SensorType typeEnum = (sensorType == "XRay") ? SensorType::XRay : SensorType::Optical;
        return std::make_shared<SensorModule>(slotsOccupied, energyConsumption, isOn, range, typeEnum);
    }
    else if (type == "WeaponModule") {
        std::string chargingDuration = moduleData["chargingDuration"].get<std::string>();
        std::chrono::milliseconds duration = parseChargingDuration(chargingDuration);
        return std::make_shared<WeaponModule>(slotsOccupied, energyConsumption, isOn, range, duration);
    }
    throw std::runtime_error("Unknown module type");
}

std::shared_ptr<Platform> loadPlatform(const json& platformData, Environment& environment) {
    const std::vector<std::string> requiredFields = {
        "type", "position", "description", "maxEnergyLevel", "slotCount"
    };
    for (const auto& field : requiredFields) {
        if (!platformData.contains(field))
            throw std::runtime_error("Missing required platform field: " + field);
    }
    if (!platformData["position"].contains("x") || !platformData["position"].contains("y")) {
        throw std::runtime_error("Position must contain x and y coordinates");
    }

    std::string type = platformData["type"].get<std::string>();
    Pair position = {platformData["position"]["x"].get<int>(), platformData["position"]["y"].get<int>()};
    std::string description = platformData["description"].get<std::string>();
    int maxEnergyLevel = platformData["maxEnergyLevel"].get<int>();
    int slotCount = platformData["slotCount"].get<int>();
    
    std::vector<std::shared_ptr<Module>> modules;
    if (platformData.contains("modules")) {
        for (auto moduleData : platformData["modules"]) {
            auto module = loadModule(moduleData);
            modules.push_back(module);
        }
    }
    if (type == "MobilePlatform") {
        if (platformData.contains("speed")) {
            int speed = platformData["speed"].get<int>();
            auto platform = std::make_shared<MobilePlatform>(position, &environment, description, maxEnergyLevel, slotCount, speed);
            for (auto module : modules)
                platform->installModule(module);
            return platform;
        }
        else throw std::runtime_error("Missing required platform fields");
    }
    else if (type == "StaticPlatform") {
        auto platform = std::make_shared<StaticPlatform>(position, &environment, description, maxEnergyLevel, slotCount);
        for (auto module : modules)
            platform->installModule(module);
        return platform;
    }
    else return nullptr;
}

void Game::loadFieldFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open())
        throw std::runtime_error("Could not open file");
    json j;
    inputFile >> j;
    Pair size = {j["field"]["width"].get<int>(), j["field"]["height"].get<int>()};
    environment_.setSize(size.x, size.y); 

    for (const auto& obstacleData : j["obstacles"]) {
        Pair position = {obstacleData["position"]["x"].get<int>(), obstacleData["position"]["y"].get<int>()};
        auto obstacle = std::make_shared<Obstacle>(position, &environment_);
        environment_.addToken(obstacle);
    }

    for (const auto& suspectData : j["suspects"]) {
        Pair position = {suspectData["position"]["x"].get<int>(), suspectData["position"]["y"].get<int>()};
        int sensorRange = suspectData["sensorRange"].get<int>();
        int speed = suspectData["speed"].get<int>();
        auto suspect = std::make_shared<Suspect>(position, &environment_, speed, sensorRange);
        environment_.addToken(suspect);
    }
    
    for (const auto& platformData : j["platforms"]) {
        auto platform = loadPlatform(platformData, environment_);
        environment_.addToken(platform);
        if (auto staticPlatform = dynamic_cast<StaticPlatform*>(platform.get()))
            ai_.addStaticPlatform(staticPlatform);
    }
}