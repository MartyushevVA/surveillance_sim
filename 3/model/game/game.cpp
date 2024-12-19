#include "game.h"

#include "../objects/objects.h"
#include "../modules/modules.h"
#include "../system/ai.h"

#include <iostream>

void Game::iterate() {
    auto env = environment_;
    if (!env) return;

    std::vector<std::shared_ptr<Suspect>> suspects;
    {
        for (const auto& [pos, token] : env->getTokens()) {
            if (auto suspect = std::dynamic_pointer_cast<Suspect>(token)) {
                suspects.push_back(suspect);
            }
        }
    }

    for (auto& suspect : suspects) {
        suspect->iterate();
    }
    
    ai_->eliminateAllSuspects();
}

void Game::pause() {
    for (auto& [pos, token] : environment_->getTokens()) {
        token->pause();
    }
}

void Game::resume() {
    for (auto& [pos, token] : environment_->getTokens()) {
        token->resume();
    }
}