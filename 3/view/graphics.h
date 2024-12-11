#pragma once

#include "../model/game/game.h"

class Graphics {
private:
    std::shared_ptr<Game> game_;

    sf::RenderWindow window_;
    GraphicsConfig config_;

    void adjustGraphicsConfig(GraphicsConfig);
    void drawObject(sf::Sprite& sprite, const Placeholder* object);

public:
    Graphics(const std::string& configPath, std::shared_ptr<Game> game) : game_(game) {
        auto config = Import::loadGraphicsConfig(configPath);
        //std::cout << "config sprites suspect size: " << config.sprites.suspect.getTexture()->getSize().x << ", " << config.sprites.suspect.getTexture()->getSize().y << std::endl;
        //std::cout << "config sprites suspect scale: " << config.sprites.suspect.getScale().x << ", " << config.sprites.suspect.getScale().y << std::endl;
        window_.create(sf::VideoMode(config.window.width, config.window.height), config.window.title);
        window_.setFramerateLimit(config.window.frameRateLimit);
        adjustGraphicsConfig(config);
        //std::cout << "config sprites suspect size: " << config_.sprites.suspect.getTexture()->getSize().x << ", " << config_.sprites.suspect.getTexture()->getSize().y << std::endl;
        //std::cout << "config sprites suspect scale: " << config_.sprites.suspect.getScale().x << ", " << config_.sprites.suspect.getScale().y << std::endl;
        
    };
    bool isWindowOpen() const { return window_.isOpen(); }
    void handleEvents();
    void render();
};