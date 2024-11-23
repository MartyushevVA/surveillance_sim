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
        if (Suspect* suspect = dynamic_cast<Suspect*>(token.get())) {
            if (MobilePlatform* predator = suspect->nearestPredatorWithinRange())
                token->move(suspect->calculateAvoidanceMove(predator->getPosition()));
            else
                token->move(suspect->calculateRandomMove());
        }
    }

    for (const auto& platform : ai_.getConnectedPlatforms()) {
        if (SensorModule* sensor = platform->findModuleOfType<SensorModule>()) {
            Report report = sensor->getSurrounding();
            if (WeaponModule* weapon = platform->findModuleOfType<WeaponModule>())
                if (Pair attackableSuspect = weapon->findAttackableSuspect(report); attackableSuspect != Pair{-1, 0})
                    weapon->attack(attackableSuspect);
            else if (MobilePlatform* officer = dynamic_cast<MobilePlatform*>(platform.get()))
                if (Pair pursuitableSuspect = officer->findPursuitableSuspect(report); pursuitableSuspect != Pair{-1, 0})
                    platform->move(officer->calculatePursuitMove(pursuitableSuspect));
                else
                    platform->move(officer->calculateRandomMove());
       }
    }
}