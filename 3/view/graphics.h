#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

#include "../model/objects/objects.h"

struct GraphicsConfig {
    struct {
        int width;
        int height;
        std::string title;
        int frameRateLimit;
        int objectSize;
    } window;

    struct Color {
        uint8_t r, g, b;
        sf::Color toSFMLColor() const { return sf::Color(r, g, b); }
    };

    Color background;
};

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

    void loadConfig(const std::string& configPath);
    void drawObject(sf::Sprite& sprite, const Placeholder* object);

public:
    Graphics(const std::string& configPath);
    bool isWindowOpen() const { return window_.isOpen(); }
    void handleEvents();
    void render();
};