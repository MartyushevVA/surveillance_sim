#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include "../model/common_types.h"
#include "../model/system/environment.h"

struct ModuleConfig {
    std::string type;
    int slotsOccupied;
    int energyConsumption;
    int range;
    struct {
        int maxSessions;
        SensorType sensorType;
        std::chrono::milliseconds chargingDuration;
    } specific;
};

struct PlatformConfig {
    std::string type;
    Pair position;
    std::string description;
    int maxEnergyLevel;
    int slotCount;
    int speed;
    std::vector<ModuleConfig> modules;
};

struct SuspectConfig {
    Pair position;
    int sensorRange;
    int speed;
};

struct ObstacleConfig {
    Pair position;
};

struct SystemConfig {
    std::chrono::milliseconds updateInterval;
    Pair size;
    std::vector<PlatformConfig> platforms;
    std::vector<SuspectConfig> suspects;
    std::vector<ObstacleConfig> obstacles;
};

class Import {
public:
    static SystemConfig loadSystemConfig(const std::string& configPath);

private:
    static ModuleConfig parseModule(const nlohmann::json& moduleJson);
    static PlatformConfig parsePlatform(const nlohmann::json& platformJson);
    static SuspectConfig parseSuspect(const nlohmann::json& suspectJson);
    static Pair parsePosition(const nlohmann::json& positionJson);
    static Pair parseSize(const nlohmann::json& sizeJson);
    static std::chrono::milliseconds parseChargingDuration(const std::string& durationStr);
};