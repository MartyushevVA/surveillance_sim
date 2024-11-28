#pragma once

#include <set>
#include <memory>
#include <map>
#include <stdexcept>
#include <cmath>

#include "common_types.h"
#include "placeholder.h"

class Obstacle : public Placeholder {
public:
    Obstacle(Pair position, Environment* environment) 
        : Placeholder(position, environment, 0) {}
        
    void setSpeed(int speed) override {}
    void update() override {}
};

class Environment {
private:
    Pair size_ = {0, 0};
    std::map<Pair, std::shared_ptr<Placeholder>> tokens_ {};

public:
    Environment() = default;
    Environment(int width, int height) {setSize(width, height);}

    Pair getSize() const {return size_;}
    void setSize(int width, int height) {size_ = {width, height};}
    
    void addToken(std::shared_ptr<Placeholder> token);
    std::shared_ptr<Placeholder> getToken(Pair position) const;
    void removeToken(Pair position);
    void moveToken(Pair from, Pair to);
    bool abilityToMove(Pair from, Pair to) const;

    std::map<Pair, std::shared_ptr<Placeholder>> getTokens() const {return tokens_;}
    
    bool isEmpty(Pair position) const;

    bool hasLineOfSight(Pair from, Pair to) const;
    double howFar(Pair from, Pair to, int range) const;
    int getDistance(Pair from, Pair to) const;
};