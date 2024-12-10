#include "controller.h"

#include <chrono>
#include <thread>

void Controller::start() {
    auto lastUpdate = std::chrono::steady_clock::now();
    while (graphics_->isWindowOpen()) {
        auto currentTime = std::chrono::steady_clock::now();
        auto deltaTime = currentTime - lastUpdate;
        graphics_->handleEvents();
        {
            game_->iterate();
            graphics_->render();
        }
        lastUpdate = currentTime;
        if (deltaTime < game_->getUpdateInterval())
            std::this_thread::sleep_for(game_->getUpdateInterval() - deltaTime);
    }
}