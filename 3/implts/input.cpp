#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "environment.h"
#include "platform.h"
#include "module_types.h"
#include "game.h"
#include "ai.h"
#include "mobile_platform.h"
#include "static_platform.h"


using json = nlohmann::json;

void loadFieldFromFile(const std::string& filename, Environment& environment) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    json j;
    inputFile >> j;
    int width = j["field"]["width"];
    int height = j["field"]["height"];
    environment.setSize(width, height);

    for (const auto& obstacleData : j["obstacles"]) {
        Pair position = {obstacleData["position"]["x"], obstacleData["position"]["y"]};
        auto obstacle = std::make_shared<Obstacle>(position, &environment);
        environment.addToken(obstacle);
    }

    for (const auto& intruderData : j["intruders"]) {
        Pair position = {intruderData["position"]["x"], intruderData["position"]["y"]};
        auto intruder = std::make_shared<Intruder>(position, &environment);
        environment.addToken(intruder);
    }

    for (const auto& platformData : j["platforms"]) {
        std::string type = platformData["type"];
        Pair position = {platformData["position"]["x"], platformData["position"]["y"]};
        std::string description = platformData["description"];
        int energyLevel = platformData["maxEnergyLevel"];
        int slotCount = platformData["slotCount"];
        int speed = platformData["speed"];
        if (type == "MobilePlatform") {
            auto platform = std::make_shared<MobilePlatform>(position, &environment, description, energyLevel, slotCount, speed);
            environment.addToken(platform);
        }
        else if (type == "StaticPlatform") {
            auto platform = std::make_shared<StaticPlatform>(position, &environment, description, energyLevel, slotCount);
            environment.addToken(platform);
            ai::addConnectedPlatform(platform);
        }
    }

    for (const auto& moduleData : j["modules"]) {
        std::string type = moduleData["type"];
        int slotsOccupied = moduleData["slotsOccupied"];
        int energyConsumption = moduleData["energyConsumption"];
        bool isOn = moduleData["isOn"];
        int range = moduleData["range"];
        if (type == "ConnectionModule") {
            int maxSessions = moduleData["maxSessions"];
            auto module = std::make_unique<ConnectionModule>(slotsOccupied, energyConsumption, isOn, range, maxSessions);
            if (moduleData["host_position"]) {
                Pair hostPosition = {moduleData["host_position"]["x"], moduleData["host_position"]["y"]};
                auto host = dynamic_cast<Platform*>(environment.getToken(hostPosition.x, hostPosition.y).get());
                if (host)
                    module->attachTo(host);
            }
            else game::addToStorage(std::move(module));
        } 
        else if (type == "SensorModule") {
            std::string sensorType = moduleData["sensorType"];
            SensorType typeEnum = (sensorType == "XRay") ? SensorType::XRay : SensorType::Optical;
            auto module = std::make_unique<SensorModule>(slotsOccupied, energyConsumption, isOn, range, typeEnum);
            if (moduleData["host_position"]) {
                Pair hostPosition = {moduleData["host_position"]["x"], moduleData["host_position"]["y"]};
                auto host = dynamic_cast<Platform*>(environment.getToken(hostPosition.x, hostPosition.y).get());
                if (host)
                    module->attachTo(host);
            }
            else game::addToStorage(std::move(module));
        }
        else if (type == "WeaponModule") {
            auto module = std::make_unique<WeaponModule>(slotsOccupied, energyConsumption, isOn, range);
            if (moduleData["host_position"]) {
                Pair hostPosition = {moduleData["host_position"]["x"], moduleData["host_position"]["y"]};
                auto host = dynamic_cast<Platform*>(environment.getToken(hostPosition.x, hostPosition.y).get());
                if (host)
                    module->attachTo(host);
            }
            else game::addToStorage(std::move(module));
        }
    }
}
