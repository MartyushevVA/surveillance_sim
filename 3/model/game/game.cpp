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
        std::shared_lock<std::shared_mutex> lock(env->getMutex());
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