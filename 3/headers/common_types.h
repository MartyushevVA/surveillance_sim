#pragma once

#include <memory>
#include <vector>

class Placeholder;
class ConnectionModule;
class Platform;
class Module;
class Environment;

struct Pair {
    int x, y;
    bool operator==(const Pair& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Pair& other) const {
        return !(*this == other);
    }
};

struct Report {
    Pair position;
    std::vector<std::shared_ptr<Placeholder>> objects;
};

struct routeNode {
    ConnectionModule* destination;
    ConnectionModule* gate;

    bool operator==(const routeNode& other) const {
        return gate == other.gate && destination == other.destination;
    }
};

enum class CellType {
    Empty,
    Obstacle,
    Platform,
    Suspect
};