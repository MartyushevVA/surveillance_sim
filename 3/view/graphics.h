#pragma once

#include "../model/game/game.h"
#include <SFML/Graphics.hpp>
#include "../import/import.h"
#include <memory>
#include <iostream>

enum class Mode {
    CONFIGURATION,
    SIMULATION
};

class Graphics {
private:
    std::shared_ptr<Game> game_;
    sf::RenderWindow window_;
    SystemConfig config_;
    GraphicsConfig graphicsConfig_;
    Mode currentMode;

    bool isSimulationPaused;
    bool isGameTyping;
    bool isEditMode = false;

    sf::Font font_;
    
    sf::Text gameInputLabel;
    sf::RectangleShape gameInputBox;
    std::string gameUserInput;

    sf::RectangleShape loadButton;
    sf::Text loadButtonText;

    sf::RectangleShape infoButton;
    sf::Text infoButtonText;

    void adjustGraphicsConfig();

    void drawObject(sf::Sprite& sprite, Pair object);
    void setupConfigurationUI();

    void renderConfigurationUI();
    void renderPreviewScreen();

    void toggleEditMode();

    void handleObjectSelection(const sf::Vector2i& mousePos);
    void handleMouseClickInConfigWindow(const sf::Vector2i& mousePos);

    void drawGrid();

public:
    Graphics(const std::string& graphicsConfigPath);
    
    SystemConfig runConfigurationWindow();
    
    void setGame(std::shared_ptr<Game> game) {game_ = game;}
    bool isWindowOpen() const { return window_.isOpen(); }
    bool isSimulationRunning() const { return currentMode == Mode::SIMULATION; }
    bool isPaused() const { return isSimulationPaused; }

    void startSimulation();
    void togglePause();
    
    void renderSimulationScreen();
    sf::Event handleSimulationEvents();
};