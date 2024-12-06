#pragma once

#include <map>

#include "../common_types.h"

class Placeholder;

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
    double getDistance(Pair from, Pair to) const;

    std::map<Pair, std::shared_ptr<Placeholder>> getArea(Pair position, int range) const;
};
