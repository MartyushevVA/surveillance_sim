#include "import.h"

SystemConfig Import::loadSystemConfig(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + configPath);
    }
    nlohmann::json j;
    file >> j;
    return parseSystemConfig(j);
}


SystemConfig Import::parseSystemConfig(const nlohmann::json& configJson) {
    SystemConfig config;
    config.updateInterval = parseChargingDuration(configJson["update_interval"]);
    config.size = parseSize(configJson["size"]);
    if (configJson.contains("obstacles")) {
    for (const auto& obstacleJson : configJson["obstacles"]) {
            config.obstacles.push_back(parseObstacle(obstacleJson));
        }
    }
    if (configJson.contains("suspects")) {
        for (const auto& suspectJson : configJson["suspects"]) {
            config.suspects.push_back(parseSuspect(suspectJson));
        }
    }
    if (configJson.contains("platforms")) {
        for (const auto& platformJson : configJson["platforms"]) {
            config.platforms.push_back(parsePlatform(platformJson));
        }
    }
    return config;
}

ModuleConfig Import::parseModule(const nlohmann::json& moduleJson) {
    const std::vector<std::string> requiredFields = {
        "type", "slotsOccupied", "energyConsumption", "range"
    };
    for (const auto& field : requiredFields) {
        if (!moduleJson.contains(field)) {
            throw std::runtime_error("Missing required module field: " + field);
        }
    }
    ModuleConfig config;
    config.type = (moduleJson["type"] == "ConnectionModule") ? ModuleType::CONNECTION : 
                  (moduleJson["type"] == "SensorModule") ? ModuleType::SENSOR : ModuleType::WEAPON;
    config.slotsOccupied = moduleJson["slotsOccupied"];
    config.energyConsumption = moduleJson["energyConsumption"];
    config.range = moduleJson["range"];
    if (moduleJson["type"] == "ConnectionModule") {
        config.specific.maxSessions = moduleJson["maxSessions"];
    }
    else if (moduleJson["type"] == "SensorModule") {
        std::string sensorType = moduleJson["sensorType"];
        config.specific.sensorType = (sensorType == "XRay") ? SensorType::XRay : SensorType::Optical;
    }
    else if (moduleJson["type"] == "WeaponModule") {
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
    std::string type = platformJson["type"];
    config.type = (type == "MobilePlatform") ? PlatformType::MOBILE : PlatformType::STATIC;
    config.position = parsePosition(platformJson["position"]);
    config.description = platformJson["description"];
    config.maxEnergyLevel = platformJson["maxEnergyLevel"];
    config.slotCount = platformJson["slotCount"];
    if (config.type == PlatformType::MOBILE) {
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

ObstacleConfig Import::parseObstacle(const nlohmann::json& obstacleJson) {
    return {
        parsePosition(obstacleJson["position"])
    };
}

Pair Import::parsePosition(const nlohmann::json& positionJson) {
    if (!positionJson.contains("x") || !positionJson.contains("y")) {
        throw std::runtime_error("Position must contain x and y coordinates");
    }
    return {positionJson["x"], positionJson["y"]};
}

Pair Import::parseSize(const nlohmann::json& sizeJson) {
    if (!sizeJson.contains("width") || !sizeJson.contains("height")) {
        throw std::runtime_error("Size must contain width and height");
    }
    return {sizeJson["width"], sizeJson["height"]};
}

std::chrono::milliseconds Import::parseChargingDuration(const std::string& durationStr) {
    int hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
    std::sscanf(durationStr.c_str(), "%d:%d:%d:%d", &hours, &minutes, &seconds, &milliseconds);
    return std::chrono::milliseconds(milliseconds + seconds * 1000 + minutes * 60000 + hours * 3600000);
}

GraphicsConfig Import::parseGraphicsConfig(const nlohmann::json& graphicsJson) {
    GraphicsConfig config;
    config.window.width = graphicsJson["window"]["width"];
    config.window.height = graphicsJson["window"]["height"];
    config.window.title = graphicsJson["window"]["title"];
    config.window.frameRateLimit = graphicsJson["window"]["framerate_limit"];
    config.window.objectSize = graphicsJson["window"]["object_size"];
    auto loadColor = [](const nlohmann::json& j) -> GraphicsConfig::Color {
        return {
            static_cast<uint8_t>(j["r"]),
            static_cast<uint8_t>(j["g"]),
            static_cast<uint8_t>(j["b"])
        };
    };
    config.background = loadColor(graphicsJson["background_color"]);

    auto loadTexture = [](const std::string& path) -> sf::Texture {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        return texture;
    };

    config.textures.suspect = loadTexture(graphicsJson["textures"]["suspect"]);
    config.textures.staticPlatform = loadTexture(graphicsJson["textures"]["static_platform"]);
    config.textures.mobilePlatform = loadTexture(graphicsJson["textures"]["mobile_platform"]);
    config.textures.obstacle = loadTexture(graphicsJson["textures"]["obstacle"]);

    config.sprites.suspect.setTexture(config.textures.suspect);
    config.sprites.staticPlatform.setTexture(config.textures.staticPlatform);
    config.sprites.mobilePlatform.setTexture(config.textures.mobilePlatform);
    config.sprites.obstacle.setTexture(config.textures.obstacle);

    float scale = static_cast<float>(config.window.objectSize) / 
            static_cast<float>(config.textures.suspect.getSize().x);
    config.sprites.suspect.setScale(scale, scale);
    scale = static_cast<float>(config.window.objectSize) / 
            static_cast<float>(config.textures.staticPlatform.getSize().x);
    config.sprites.staticPlatform.setScale(scale, scale);
    scale = static_cast<float>(config.window.objectSize) / 
            static_cast<float>(config.textures.mobilePlatform.getSize().x);
    config.sprites.mobilePlatform.setScale(scale, scale);
    scale = static_cast<float>(config.window.objectSize) / 
            static_cast<float>(config.textures.obstacle.getSize().x);
    config.sprites.obstacle.setScale(scale, scale);
    return config;
}

GraphicsConfig Import::loadGraphicsConfig(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open graphics config file: " + configPath);
    }
    nlohmann::json j;
    file >> j;
    return parseGraphicsConfig(j);
}