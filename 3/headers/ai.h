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
    std::vector<Suspect*> suspects_ {};

public:
    AI(Environment* environment) : environment_(environment) {};

    std::vector<Platform*> getAllConnectedPlatforms() const {return allConnectedPlatforms_;}
    void addStaticPlatform(Platform* platform) {staticPlatforms_.push_back(platform);}
    void updateConnections();
    void addSuspect(Suspect* suspect) {suspects_.push_back(suspect);}
    void updateSuspects();
};


/*
при обнаружении каким либо сенсором противника, все мобильные роботы в юрисдикции направляются к нему
нужно организовать приоритетную очередь преступников по наименьшей сумме расстояний от них до всех роботов*/