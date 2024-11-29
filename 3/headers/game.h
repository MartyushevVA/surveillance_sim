#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "environment.h"
#include "ai.h"

class Module;

class Game {
private:
    sf::RenderWindow window_;
    static const int WINDOW_WIDTH = 1000;
    static const int WINDOW_HEIGHT = 1000;
    static const int RATIO = 10;

    Environment environment_ {};
    AI ai_ {&environment_};

    void handleEvents();
    void render();
    
    void updateSuspects();

public:
    Game(const std::string& filename) {
        loadFieldFromFile(filename);
    };

    void loadFieldFromFile(const std::string& filename);

    void start();
};