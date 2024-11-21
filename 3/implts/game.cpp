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
            if (MobilePlatform* predator = intruder->nearestPredatorWithinRange())
                token->move(intruder->calculateAvoidanceMove(predator->getPosition()));
            else
                token->move(intruder->calculateRandomMove());
        }
    }

    ai_.refreshConnectedPlatforms();

    for (const auto& platform : ai_.getConnectedPlatforms()) {
        if (SensorModule* sensor = platform->findModuleOfType<SensorModule>()) {
            Report report = sensor->getSurrounding();
            if (WeaponModule* weapon = platform->findModuleOfType<WeaponModule>())
                if (Pair attackableIntruder = weapon->findAttackableIntruder(report); attackableIntruder != Pair{-1, 0})
                    weapon->attack(attackableIntruder);
            else if (MobilePlatform* officer = dynamic_cast<MobilePlatform*>(platform.get()))
                if (Pair pursuitableIntruder = officer->findPursuitableIntruder(report); pursuitableIntruder != Pair{-1, 0})
                    platform->move(officer->calculatePursuitMove(pursuitableIntruder));
                else
                    platform->move(officer->calculateRandomMove());
    }
}