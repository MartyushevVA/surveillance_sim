#pragma once

#include <memory>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>

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


/*
при обнаружении каким либо сенсором противника, все мобильные роботы в юрисдикции направляются к нему
нужно организовать приоритетную очередь преступников по наименьшей сумме расстояний от них до всех роботов*/