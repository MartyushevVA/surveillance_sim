#pragma once

#include <memory>
#include <string>
#include <vector>

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
    std::vector<std::shared_ptr<Placeholder>> objects;
};

enum SensorType {
    Optical,
    XRay
};

struct routeNode {
    ConnectionModule* destination;
    ConnectionModule* gate;

    bool operator==(const routeNode& other) const {
        return destination == other.destination;
    }
    routeNode(ConnectionModule* gate, ConnectionModule* destination) : destination(destination), gate(gate) {}
};

enum class CellType {
    Empty,
    Obstacle,
    Platform,
    Suspect
};