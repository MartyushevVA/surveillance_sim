#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include "../model/common_types.h"

class Import {
public:
    static SystemConfig loadSystemConfig(const std::string& configPath);
    static GraphicsConfig loadGraphicsConfig(const std::string& configPath);

private:
    static SystemConfig parseSystemConfig(const nlohmann::json& configJson);
    static ModuleConfig parseModule(const nlohmann::json& moduleJson);
    static PlatformConfig parsePlatform(const nlohmann::json& platformJson);
    static SuspectConfig parseSuspect(const nlohmann::json& suspectJson);
    static ObstacleConfig parseObstacle(const nlohmann::json& obstacleJson);
    static Pair parsePosition(const nlohmann::json& positionJson);
    static Pair parseSize(const nlohmann::json& sizeJson);
    static std::chrono::milliseconds parseChargingDuration(const std::string& durationStr);

    static GraphicsConfig parseGraphicsConfig(const nlohmann::json& graphicsJson);
};
