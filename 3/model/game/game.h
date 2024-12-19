#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <memory>

#include "../system/ai.h"
#include "../system/environment.h"
#include "../../import/import.h"

#include <iostream>

class Game {
private:
    std::shared_ptr<Environment> environment_;
    std::shared_ptr<AI> ai_;
    std::chrono::milliseconds updateInterval_;

public:
    Game(const SystemConfig& config) {
        environment_ = std::make_shared<Environment>(Pair{config.size.x, config.size.y});
        environment_->initialize(config);
        ai_ = std::make_shared<AI>(environment_);
        ai_->syncHosts();
        updateInterval_ = config.updateInterval;
    }
    void iterate();

    void pause();
    void resume();

    std::shared_ptr<Environment> getEnvironment() const { return environment_; }
    std::shared_ptr<AI> getAI() const { return ai_; }
    std::chrono::milliseconds getUpdateInterval() const { return updateInterval_; }
};
