#pragma once

#include <memory>

#include "../game/game.h"
#include "../../view/graphics.h"

class Controller {
private:
    std::shared_ptr<Game> game_;
    std::unique_ptr<Graphics> graphics_;

    SystemConfig config_;

    void startSimulation();

public:
    Controller();
    void start();
};