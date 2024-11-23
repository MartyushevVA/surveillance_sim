#pragma once

#include <memory>
#include <queue>
#include <set>
#include <vector>

#include "platform.h"
#include "environment.h"
#include "interfaces.h"
#include "module_types.h"

class AI {
private:
    Environment* environment_;
    std::vector<std::shared_ptr<Platform>> connectedPlatforms_ {};

public:
    AI(Environment* environment) : environment_(environment) {};

    Environment* getEnvironment() const {return environment_;}
    
    void addConnectedPlatform(std::shared_ptr<Platform> platform) {connectedPlatforms_.push_back(platform);}
    void removeConnectedPlatform(std::shared_ptr<Platform> platform) {connectedPlatforms_.erase(std::remove(connectedPlatforms_.begin(), connectedPlatforms_.end(), platform), connectedPlatforms_.end());}
    std::vector<std::shared_ptr<Platform>> getConnectedPlatforms() const {return connectedPlatforms_;}
};