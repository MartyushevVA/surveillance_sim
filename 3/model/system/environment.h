#pragma once

#include <map>
#include <shared_mutex>
#include <memory>

#include "../common_types.h"

class Placeholder;

class Environment {
private:
    Pair size_;
    std::map<Pair, std::shared_ptr<Placeholder>> tokens_;
    mutable std::shared_mutex environmentMutex_;
    mutable std::vector<std::shared_mutex> mutexes_;
    
public:
    Environment() = delete;
    Environment(Pair size) : 
        size_{size},
        mutexes_(size.x * size.y) {}
    
    Pair getSize() const {return size_;}
    void addToken(std::shared_ptr<Placeholder> token);
    std::shared_ptr<Placeholder> getToken(Pair position) const;
    void removeToken(Pair position);
    void moveToken(Pair from, Pair to);
    bool abilityToMove(Pair from, Pair to) const;

    std::map<Pair, std::shared_ptr<Placeholder>> getTokens() const {return tokens_;}
    
    bool isEmpty(Pair position) const;

    bool hasLineOfSight(Pair from, Pair to) const;
    std::vector<Pair> getLine(Pair from, Pair to) const;
    double isInRange(Pair from, Pair to, int range) const;
    double calculateDistance(Pair from, Pair to) const;

    std::vector<Pair> representArea(Pair position, int range) const;
    std::map<Pair, std::shared_ptr<Placeholder>> getArea(Pair position, int range) const;

    template <typename T>
    std::shared_ptr<T> getClosestOfType(Pair position, std::map<Pair, std::shared_ptr<Placeholder>> area) const {
        std::shared_ptr<T> closest = nullptr;
        for (auto [pos, token] : area)
            if (auto typed_token = std::dynamic_pointer_cast<T>(token))
                if (!closest || calculateDistance(position, pos) < calculateDistance(position, closest->getPosition()))
                    closest = typed_token;
        return closest;
    }
};
