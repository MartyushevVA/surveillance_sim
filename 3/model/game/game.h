#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <memory>

#include "../system/ai.h"
#include "../system/environment.h"
#include "../../import/import.h"

class Game {
private:
    std::shared_ptr<Environment> environment_;
    std::shared_ptr<AI> ai_;
    std::chrono::milliseconds updateInterval_;

public:
    Game(const std::string& gameConfigFile) {
        auto config = Import::loadSystemConfig(gameConfigFile);
        environment_ = std::make_shared<Environment>(config);
        ai_ = std::make_shared<AI>(environment_);
        updateInterval_ = config.updateInterval;
    }
    void iterate();
    std::chrono::milliseconds getUpdateInterval() const { return updateInterval_; }
};