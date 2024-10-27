#pragma once

#include <vector>
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
    Pair size_ = {0, 0};
    std::vector<std::unique_ptr<Placeholder>> tokens_;

public:
    Environment() = default;
    Environment(int width, int height) : size_({width, height}) {}

    Pair getSize() const;
    void setSize(int width, int height);
    const std::vector<std::unique_ptr<Placeholder>>& getTokens() const;
    void addToken(std::unique_ptr<Placeholder> token);
    void removeToken(Placeholder* token);
    
    CellType getCellType(int x, int y) const;
    void setCellType(int x, int y, CellType type);
};