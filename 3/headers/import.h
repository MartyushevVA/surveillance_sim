#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <chrono>

#include "module_types.h"

struct ModuleConfig {
    std::string type;
    int slotsOccupied;
    int energyConsumption;
    bool isOn;
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

struct FieldConfig {
    struct {
        int width;
        int height;
    } size;
    std::vector<Pair> obstacles;
    std::vector<SuspectConfig> suspects;
    std::vector<PlatformConfig> platforms;
};

class Import {
public:
    static FieldConfig importFieldConfig(const std::string& configPath);

private:
    static ModuleConfig parseModule(const nlohmann::json& moduleJson);
    static PlatformConfig parsePlatform(const nlohmann::json& platformJson);
    static SuspectConfig parseSuspect(const nlohmann::json& suspectJson);
    static Pair parsePosition(const nlohmann::json& positionJson);
    static std::chrono::milliseconds parseChargingDuration(const std::string& durationStr);
};