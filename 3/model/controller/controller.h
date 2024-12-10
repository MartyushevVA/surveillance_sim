#pragma once

#include <memory>

#include "../game/game.h"
#include "../../view/graphics.h"

class Controller {
private:
    std::shared_ptr<Game> game_;
    std::unique_ptr<Graphics> graphics_;

public:
    Controller(const std::string& gameConfigFile, const std::string& graphicsConfigFile) :
        game_(std::make_shared<Game>(gameConfigFile)),
        graphics_(std::make_unique<Graphics>(graphicsConfigFile, game_)) {}

    void start();

};