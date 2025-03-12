#include "controller.h"

#include <chrono>
#include <thread>
#include <iostream>

Controller::Controller() : 
    game_(nullptr),
    graphics_(std::make_unique<Graphics>("configs/graphics.json")) {}

void Controller::startSimulation() {
    game_ = std::make_shared<Game>(config_);
    graphics_->setGame(game_);
    graphics_->startSimulation();
}

void Controller::start() {
    config_ = graphics_->runConfigurationWindow();

    startSimulation();

    auto lastUpdate = std::chrono::steady_clock::now();
    while (graphics_->isWindowOpen() && graphics_->isSimulationRunning()) {
        sf::Event event = graphics_->handleSimulationEvents();
        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::P) {
                if (graphics_->isPaused())
                    game_->pause();
                else
                    game_->resume();
            }

        if (!graphics_->isPaused()) {
            auto currentTime = std::chrono::steady_clock::now();
            auto deltaTime = currentTime - lastUpdate;
            game_->iterate();
            graphics_->renderSimulationScreen();
            lastUpdate = currentTime;
            if (deltaTime < game_->getUpdateInterval())
                std::this_thread::sleep_for(game_->getUpdateInterval() - deltaTime);
        }
        else
            graphics_->renderSimulationScreen();
    }
}