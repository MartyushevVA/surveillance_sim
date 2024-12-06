#pragma once

#include <vector>

class Environment;
class Platform;
class StaticPlatform;
class MobilePlatform;
class Suspect;
class Placeholder;

class AI {
private:
    Environment* environment_;
    std::vector<StaticPlatform*> staticPlatforms_ {};
    std::vector<MobilePlatform*> allConnectedPlatforms_ {};
    std::vector<Placeholder*> spottedSuspects_ {};

    void updateNetworkForest();
    void addSuspect(Placeholder* suspect) {spottedSuspects_.push_back(suspect);}
    void updateSpottedSuspects();

public:
    AI(Environment* environment) : environment_(environment) {};

    std::vector<Placeholder*> getSpottedSuspects() {return spottedSuspects_;}
    void addStaticPlatform(StaticPlatform* platform) {staticPlatforms_.push_back(platform);}
    void eliminateAllSuspects();
};