#pragma once

#include <memory>
#include <vector>

class Platform;
class Environment;

class AI {
private:
    std::vector<Platform*> ConnectedPlatforms_ {};
    Environment* environment_ = nullptr;
public:
    AI(Environment* environment) : environment_(environment) {};
    void eliminateAllIntruders();
};