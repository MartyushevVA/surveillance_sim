#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include "../model/game/game.h"

class Graphics {
private:
    std::shared_ptr<Game> game_;

    sf::RenderWindow window_;
    GraphicsConfig config_;

    struct {
        sf::Texture suspect;
        sf::Texture staticPlatform;
        sf::Texture mobilePlatform;
        sf::Texture obstacle;
    } textures_;

    struct {
        sf::Sprite suspect;
        sf::Sprite staticPlatform;
        sf::Sprite mobilePlatform;
        sf::Sprite obstacle;
    } sprites_;

    void drawObject(sf::Sprite& sprite, const Placeholder* object);

public:
    Graphics(const std::string& configPath, std::shared_ptr<Game> game) : game_(game) {
        auto config = Import::loadGraphicsConfig(configPath);
        window_.create(sf::VideoMode(config.window.width, config.window.height), config.window.title);
        window_.setFramerateLimit(config.window.frameRateLimit);
        config_ = config;
    };
    bool isWindowOpen() const { return window_.isOpen(); }
    void handleEvents();
    void render();
};