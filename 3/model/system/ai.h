#pragma once

#include <vector>
#include <memory>

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
    std::vector<Placeholder*> spottedSuspects_ {};

    void updateNetworkForest();
    void addSuspect(Placeholder* suspect) {spottedSuspects_.push_back(suspect);}
    void updateSpottedSuspects();

public:
    AI(Environment* environment) : environment_(environment) {};

    std::vector<Placeholder*> getSpottedSuspects() {return spottedSuspects_;}
    void addStaticPlatform(std::shared_ptr<StaticPlatform> platform) {if (platform) staticPlatforms_.push_back(platform);}
    void eliminateAllSuspects();
};