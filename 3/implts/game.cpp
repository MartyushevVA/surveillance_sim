#include "game.h"

void Game::start() {
    bool isRunning = true;
    while (isRunning) {
        update();
        render();
    }
}

void Game::stop() {

}

void Game::update() {
    for (const auto& token : environment_.getTokens()) {
        if (Intruder* intruder = dynamic_cast<Intruder*>(token.get())) {
            if (MobilePlatform* predator = intruder->predatorNearby(intruder->getPosition()))
                token->move(intruder->calculateAvoidanceMove(predator->getPosition()));
            else
                token->move(intruder->calculateRandomMove());
        }
    }

    for (const auto& platform : ai_.getConnectedPlatforms()) {
        if (SensorModule* sensor = platform->findModuleOfType<SensorModule>())
            auto report = sensor->getSurrounding();
            
    }
}