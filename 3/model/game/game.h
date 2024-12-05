#pragma once

#include "../system/ai.h"
#include "../system/environment.h"
#include "../../import/import.h"
#include "../../view/graphics.h"

class Game {
private:
    AI ai_;
    Environment environment_;
    Graphics graphics_;
    std::chrono::milliseconds updateInterval_;
    
    void initializeField(const GameConfig& config);

public:
    Game(const std::string& gameConfigFile, const std::string& graphicsConfigFile) :
        ai_(&environment_),
        graphics_(graphicsConfigFile) {
        initializeField(Import::importGameConfig(gameConfigFile));
    }    
    void start();
    void updateSuspects();
};