#pragma once

#include <vector>
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
    std::vector<Placeholder*> tokens_ = {};

public:
    Environment(int width, int height, std::vector<Placeholder*> members);

    Pair getSize() const;
    void setSize(int width, int height);
    std::vector<Placeholder*> getTokens() const;
    void setTokens(std::vector<Placeholder*> tokens);
    
    CellType getCellType(int x, int y) const;
    void setCellType(int x, int y, CellType type);
};