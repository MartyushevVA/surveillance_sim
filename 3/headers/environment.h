#pragma once

#include <set>
#include <memory>
#include <stdexcept>
#include <cmath>

#include "placeholder.h"
#include "intruder.h"
#include "platform.h"

enum class CellType {
    Empty,
    Obstacle,
    Platform,
    Intruder
};

class Obstacle : public Placeholder {
public:
    void setSpeed(int speed) override {speed_ = 0;}
};

class Environment {
private:
    Pair size_ = {0, 0};
    std::set<std::shared_ptr<Placeholder>> tokens_ {};

public:
    Environment() = default;
    Environment(int width, int height) : size_({width, height}) {}

    Pair getSize() const {return size_;}
    void setSize(int width, int height) {
        if (width > 0 && height > 0)
            size_ = {width, height};
    }
    
    void addToken(std::shared_ptr<Placeholder> token);
    std::shared_ptr<Placeholder> removeToken(int x, int y);

    std::set<std::shared_ptr<Placeholder>> getTokens() const {return tokens_;}
    std::shared_ptr<Placeholder> getToken(int x, int y) const;

    CellType getCellType(int x, int y) const;
    void setCellType(int x, int y, CellType type);

    bool hasLineOfSight(Pair from, Pair to) const;
};