#include "import.h"

FieldConfig Import::importFieldConfig(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + configPath);
    }
    nlohmann::json j;
    file >> j;
    FieldConfig config;
    config.size.width = j["field"]["width"];
    config.size.height = j["field"]["height"];
    for (const auto& obstacleJson : j["obstacles"]) {
        config.obstacles.push_back(parsePosition(obstacleJson["position"]));
    }
    for (const auto& suspectJson : j["suspects"]) {
        config.suspects.push_back(parseSuspect(suspectJson));
    }
    for (const auto& platformJson : j["platforms"]) {
        config.platforms.push_back(parsePlatform(platformJson));
    }
    return config;
}

ModuleConfig Import::parseModule(const nlohmann::json& moduleJson) {
    const std::vector<std::string> requiredFields = {
        "type", "slotsOccupied", "energyConsumption", "isOn", "range"
    };
    for (const auto& field : requiredFields) {
        if (!moduleJson.contains(field)) {
            throw std::runtime_error("Missing required module field: " + field);
        }
    }
    ModuleConfig config;
    config.type = moduleJson["type"];
    config.slotsOccupied = moduleJson["slotsOccupied"];
    config.energyConsumption = moduleJson["energyConsumption"];
    config.isOn = moduleJson["isOn"];
    config.range = moduleJson["range"];
    if (config.type == "ConnectionModule") {
        config.specific.maxSessions = moduleJson["maxSessions"];
    }
    else if (config.type == "SensorModule") {
        std::string sensorType = moduleJson["sensorType"];
        config.specific.sensorType = (sensorType == "XRay") ? SensorType::XRay : SensorType::Optical;
    }
    else if (config.type == "WeaponModule") {
        config.specific.chargingDuration = parseChargingDuration(moduleJson["chargingDuration"]);
    }
    return config;
}

PlatformConfig Import::parsePlatform(const nlohmann::json& platformJson) {
    const std::vector<std::string> requiredFields = {
        "type", "position", "description", "maxEnergyLevel", "slotCount"
    };
    for (const auto& field : requiredFields) {
        if (!platformJson.contains(field)) {
            throw std::runtime_error("Missing required platform field: " + field);
        }
    }
    PlatformConfig config;
    config.type = platformJson["type"];
    config.position = parsePosition(platformJson["position"]);
    config.description = platformJson["description"];
    config.maxEnergyLevel = platformJson["maxEnergyLevel"];
    config.slotCount = platformJson["slotCount"];
    if (config.type == "MobilePlatform") {
        if (!platformJson.contains("speed")) {
            throw std::runtime_error("Speed is required for mobile platforms");
        }
        config.speed = platformJson["speed"];
    }
    if (platformJson.contains("modules")) {
        for (const auto& moduleJson : platformJson["modules"]) {
            config.modules.push_back(parseModule(moduleJson));
        }
    }
    return config;
}

SuspectConfig Import::parseSuspect(const nlohmann::json& suspectJson) {
    return {
        parsePosition(suspectJson["position"]),
        suspectJson["sensorRange"],
        suspectJson["speed"]
    };
}

Pair Import::parsePosition(const nlohmann::json& positionJson) {
    if (!positionJson.contains("x") || !positionJson.contains("y")) {
        throw std::runtime_error("Position must contain x and y coordinates");
    }
    return {positionJson["x"], positionJson["y"]};
}

std::chrono::milliseconds Import::parseChargingDuration(const std::string& durationStr) {
    int hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
    std::sscanf(durationStr.c_str(), "%d:%d:%d:%d", &hours, &minutes, &seconds, &milliseconds);
    return std::chrono::milliseconds(milliseconds + seconds * 1000 + minutes * 60000 + hours * 3600000);
}