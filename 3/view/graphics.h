#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

#include "../model/objects/objects.h"

class Environment;
class MobilePlatform;
class Obstacle;
class StaticPlatform;
class Suspect;

struct GraphicsConfig {
    struct {
        int width;
        int height;
        std::string title;
        int frameRateLimit;
        int pixelRatio;
    } window;

    struct Color {
        uint8_t r, g, b;
        sf::Color toSFMLColor() const { return sf::Color(r, g, b); }
    };

    struct {
        Color background;
        Color suspect;
        Color staticPlatform;
        Color mobilePlatform;
        Color obstacle;
    } colors;
};

class Graphics {
private:
    sf::RenderWindow window_;
    GraphicsConfig config_;

    void loadConfig(const std::string& configPath);
    void drawSuspect(const Suspect* suspect);
    void drawStaticPlatform(const StaticPlatform* platform);
    void drawMobilePlatform(const MobilePlatform* platform);
    void drawObstacle(const Obstacle* obstacle);

public:
    Graphics(const std::string& configPath);
    bool isWindowOpen() const { return window_.isOpen(); }
    void handleEvents();
    void render(const Environment& environment);
};