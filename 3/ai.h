#pragma once

#include <memory>
#include <vector>

class Platform;
class Environment;

class AI {
private:
    std::vector<std::shared_ptr<Platform>> ConnectedPlatforms_ = {};
    std::shared_ptr<Environment> environment_ = nullptr;
public:
    AI(std::shared_ptr<Environment> environment);
    void eliminateAllIntruders();
};