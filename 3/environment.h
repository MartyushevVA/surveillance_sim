#pragma once

#include <vector>

enum class CellType {
    Empty,
    Obstacle,
    Platform,
    Intruder
};

class Environment {
private:
    int width_;
    int height_;
    std::vector<std::vector<CellType>> grid_;

public:
    Environment(int width, int height);
    std::pair<int, int> getSize() const;
    void setSize(int width, int height);
    CellType getCellType(int x, int y) const;
    void setCellType(int x, int y, CellType type);
};