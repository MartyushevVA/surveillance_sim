#pragma once

#include <memory>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>

class Environment;
class Platform;

class AI {
private:
    Environment* environment_;
    std::vector<Platform*> staticPlatforms_ {};
    std::vector<Platform*> temporarilyConnectedPlatforms_ {};

public:
    AI(Environment* environment) : environment_(environment) {};

    Environment* getEnvironment() const {return environment_;}
    std::vector<Platform*> getStaticPlatforms() const {return staticPlatforms_;}
    std::vector<Platform*> getTemporarilyConnectedPlatforms() const {return temporarilyConnectedPlatforms_;}
    void addStaticPlatform(Platform* platform) {staticPlatforms_.push_back(platform);}
    void updateConnections();
};


/*
при обнаружении каким либо сенсором противника, все мобильные роботы в юрисдикции направляются к нему*/