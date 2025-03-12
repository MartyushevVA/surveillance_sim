#pragma once

#include <map>
#include <utility>
#include <shared_mutex>

#include <memory>
#include <iostream>

#include "../common_types.h"

class Placeholder;

class Environment : public std::enable_shared_from_this<Environment> {
private:
    Pair size_;
    std::map<Pair, std::shared_ptr<Placeholder>> tokens_;
    bool initialized_ = false;
    mutable std::shared_mutex mutex_;


    void checkInitialized() const {
        if (!initialized_)
            throw std::runtime_error("Environment is not initialized");
    }
    
public:
    Environment() = delete;
    explicit Environment(Pair size) : size_{size} {}
    
    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;
    
    void initialize(SystemConfig config);
    Pair getSize() const {
        checkInitialized();
        return size_;
    }

    std::shared_mutex& getMutex() {return mutex_;}
    void addToken(std::shared_ptr<Placeholder> token);
    std::shared_ptr<Placeholder> getToken(Pair position) const;
    bool removeToken(Pair position);
    void moveToken(Pair from, Pair to);
    bool abilityToMove(Pair from, Pair to) const;
    bool isValidCoords(Pair position) const {
        return position.x >= 0 && position.y >= 0 && position.x < size_.x && position.y < size_.y;
    };

    std::map<Pair, std::shared_ptr<Placeholder>> getTokens() const {return tokens_;}
    bool isEmpty(Pair position) const;

    bool hasLineOfSight(Pair from, Pair to) const;
    std::vector<Pair> getLine(Pair from, Pair to) const;
    double isInRange(Pair from, Pair to, int range) const;
    double calculateDistance(Pair from, Pair to) const;

    std::vector<Pair> getAreaCords(Pair position, int range) const;
    std::map<Pair, std::shared_ptr<Placeholder>> getArea(Pair position, int range) const;

    template <typename T>
    std::pair<Pair, std::shared_ptr<T>> getClosestOfType(Pair position, std::map<Pair, std::shared_ptr<T>> area) const {
        std::shared_ptr<T> closest = nullptr;
        for (auto [pos, token] : area)
            if (auto typed_token = std::dynamic_pointer_cast<T>(token))
                if (!closest || calculateDistance(position, pos) < calculateDistance(position, closest->getPosition()))
                    closest = typed_token;
        return std::make_pair(closest->getPosition(), closest);
    };

    template <typename T>
    std::map<Pair, std::shared_ptr<T>> getEnemies(std::map<Pair, std::shared_ptr<Placeholder>> area) const {
        std::map<Pair, std::shared_ptr<T>> enemies;
        for (auto [pos, placeholder] : area) {
            if (auto enemy = std::dynamic_pointer_cast<T>(placeholder))
                enemies[pos] = enemy;
        }
        return enemies;
    }
};
