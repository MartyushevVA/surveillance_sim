#pragma once

#include <map>
#include <shared_mutex>

#include "../common_types.h"

class Placeholder;

class Environment {
private:
    Pair size_ = {0, 0};
    std::map<Pair, std::shared_ptr<Placeholder>> tokens_ {};
    
public:
    mutable std::mutex mutex_;

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
    double isInRange(Pair from, Pair to, int range) const;
    double calculateDistance(Pair from, Pair to) const;

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
