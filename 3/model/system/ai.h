#pragma once

#include <vector>

class Environment;
class Platform;
class Suspect;

class AI {
private:
    Environment* environment_;
    std::vector<Platform*> staticPlatforms_ {};
    std::vector<Platform*> allConnectedPlatforms_ {};
    std::vector<Suspect*> spottedSuspects_ {};

    void updateNetworkForest();
    void addSuspect(Suspect* suspect) {spottedSuspects_.push_back(suspect);}
    void updateSpottedSuspects();

public:
    AI(Environment* environment) : environment_(environment) {};

    void addStaticPlatform(Platform* platform) {staticPlatforms_.push_back(platform);}
    void eliminateAllSuspects();
};