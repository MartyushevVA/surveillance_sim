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
    
    void updateSuspects();
    void initializeField(const FieldConfig& config);

public:
    Game(const std::string& fieldFile, const std::string& graphicsConfigFile) :
        ai_(&environment_),
        graphics_(graphicsConfigFile) {
        initializeField(Import::importFieldConfig(fieldFile));
    }    
    void start();
};