#pragma once

#include <vector>
#include <map>
#include <memory>

#include "../common_types.h"

class Environment;
class Platform;
class StaticPlatform;
class MobilePlatform;
class Suspect;
class Placeholder;

class AI {
private:
    Environment* environment_;
    std::vector<std::shared_ptr<StaticPlatform>> staticPlatforms_ {};
    std::vector<Platform*> allConnectedPlatforms_ {};
    std::map<Pair, std::shared_ptr<Placeholder>> spottedSuspects_ {};

    std::mutex networkMutex_;

    

public:
    AI(Environment* environment) : environment_(environment) {};

    void addStaticPlatform(std::shared_ptr<StaticPlatform> platform) {if (platform) staticPlatforms_.push_back(platform);}
    void eliminateAllSuspects();
    void getNetworkForest();
    const std::vector<Platform*>& getAllConnectedPlatforms() const {return allConnectedPlatforms_;}
    
    void addSuspects(std::map<Pair, std::shared_ptr<Placeholder>> suspects);
    void removeSuspect(std::shared_ptr<Placeholder> suspect);
    const std::map<Pair, std::shared_ptr<Placeholder>>& getSuspects() const {return spottedSuspects_;}
};