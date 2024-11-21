#pragma once

#include <set>
#include <memory>
#include <stdexcept>
#include <cmath>

#include "placeholder.h"
#include "suspect.h"
#include "platform.h"

enum class CellType {
    Empty,
    Obstacle,
    Platform,
    Suspect
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
    Environment(int width, int height) {setSize(width, height);}

    Pair getSize() const {return size_;}
    void setSize(int width, int height) {
        if (width <= 0 || height <= 0)
            throw std::invalid_argument("Width and height must be positive");
        size_ = {width, height};
    }
    
    void addToken(std::shared_ptr<Placeholder> token);
    std::shared_ptr<Placeholder> extractToken(Pair position);

    std::set<std::shared_ptr<Placeholder>> getTokens() const {return tokens_;}
    std::shared_ptr<Placeholder> getToken(Pair position) const;

    CellType getCellType(Pair position) const;

    bool hasLineOfSight(Pair from, Pair to) const;
    double howFar(Pair from, Pair to, int range) const;
};