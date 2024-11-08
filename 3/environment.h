#pragma once

#include <set>
#include <memory>
#include "placeholder.h"

enum class CellType {
    Empty,
    Obstacle,
    Platform,
    Intruder
};

class Obstacle : public Placeholder {};

class Environment {
private:
    struct PlaceholderPtrCompare {
        bool operator()(const std::unique_ptr<Placeholder>& a, 
                       const std::unique_ptr<Placeholder>& b) const {
            return *a < *b;
        }
    };
    Pair size_ = {0, 0};
    std::set<std::unique_ptr<Placeholder>, PlaceholderPtrCompare> tokens_ {};

public:
    Environment() = default;
    Environment(int width, int height) : size_({width, height}) {}

    Pair getSize() const;
    void setSize(int width, int height);
    
    void addToken(std::unique_ptr<Placeholder> token);
    std::unique_ptr<Placeholder> removeToken(const Placeholder& token);

    std::set<std::unique_ptr<Placeholder>, PlaceholderPtrCompare> getTokens() const;
    
    CellType getCellType(int x, int y) const;
    void setCellType(int x, int y, CellType type);
};