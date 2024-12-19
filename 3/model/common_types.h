#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <chrono>

#include <SFML/Graphics.hpp>

class ConnectionModule;
class Placeholder;

struct Pair {
    int x, y;
    bool operator==(const Pair& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Pair& other) const {
        return !(*this == other);
    }
    bool operator<(const Pair& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

struct Report {
    Pair position;
    std::map<Pair, std::shared_ptr<Placeholder>> objects;
};

enum SensorType {
    Optical,
    XRay
};

struct routeNode {
    std::weak_ptr<ConnectionModule> destination = {};
    std::weak_ptr<ConnectionModule> gate = {};

    bool operator==(const routeNode& other) const {
        return destination.lock() == other.destination.lock();
    }
    routeNode() = default;
    routeNode(std::weak_ptr<ConnectionModule> gate, std::weak_ptr<ConnectionModule> destination) : destination(destination), gate(gate) {}
};

enum class GameState {
    RUNNING,
    PAUSED
};

enum class PlaceholderType {
    Suspect,
    MobilePlatform,
    StaticPlatform,
    Obstacle
};

enum class PlatformType {
    STATIC,
    MOBILE
};

enum class ModuleType {
    CONNECTION,
    SENSOR,
    WEAPON
};

struct ModuleConfig {
    ModuleType type;
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
    PlatformType type;
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

struct GraphicsConfig {
    struct {
        int width;
        int height;
        std::string title;
        int frameRateLimit;
        int objectSize;
    } window;

    struct Color {
        uint8_t r, g, b;
        sf::Color toSFMLColor() const { return sf::Color(r, g, b); }
    };

    Color background;

    struct {
        sf::Texture suspect {};
        sf::Texture staticPlatform {};
        sf::Texture mobilePlatform {};
        sf::Texture obstacle {};
    } textures;

    struct {
        sf::Sprite suspect {};
        sf::Sprite staticPlatform {};
        sf::Sprite mobilePlatform {};
        sf::Sprite obstacle {};
    } sprites;

    struct {
        std::string suspectNameOfFile {};
        std::string staticPlatformNameOfFile {};
        std::string mobilePlatformNameOfFile {};
        std::string obstacleNameOfFile {};
    } sources;
};