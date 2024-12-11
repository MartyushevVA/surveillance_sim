#include "game.h"

#include "../objects/objects.h"
#include "../modules/modules.h"
#include "../system/ai.h"

#include <iostream>

void Game::iterate() {
    for (const auto& [pos, token] : environment_->getTokens())
        if (auto suspect = std::dynamic_pointer_cast<Suspect>(token)) {
            suspect->iterate();
        }
    ai_->eliminateAllSuspects();
}