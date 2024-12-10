#include "game.h"

#include "../objects/objects.h"
#include "../modules/modules.h"
#include "../system/ai.h"

#include <iostream>

void Game::iterate() {
    std::cout << "Game::iterate" << std::endl;
    for (const auto& [pos, token] : environment_->getTokens())
        if (auto suspect = std::dynamic_pointer_cast<Suspect>(token)) {
            std::cout << "suspect->iterate at " << pos.x << ", " << pos.y << std::endl;
            suspect->iterate();
            std::cout << "suspect->iterate done" << std::endl;
        }
    std::cout << "ai_->eliminateAllSuspects" << std::endl;
    ai_->eliminateAllSuspects();
}