#include "graphics.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include "../model/system/environment.h"

#include "../model/common_types.h"

void Graphics::handleEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();
    }
}

void Graphics::adjustGraphicsConfig(GraphicsConfig config) {
    config_ = config;

    auto loadTexture = [](const std::string& path) -> sf::Texture {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        return texture;
    };

    config_.textures.suspect = loadTexture(config_.sources.suspectNameOfFile);
    config_.textures.staticPlatform = loadTexture(config_.sources.staticPlatformNameOfFile);
    config_.textures.mobilePlatform = loadTexture(config_.sources.mobilePlatformNameOfFile);
    config_.textures.obstacle = loadTexture(config_.sources.obstacleNameOfFile);

    config_.sprites.suspect.setTexture(config_.textures.suspect);
    config_.sprites.staticPlatform.setTexture(config_.textures.staticPlatform);
    config_.sprites.mobilePlatform.setTexture(config_.textures.mobilePlatform);
    config_.sprites.obstacle.setTexture(config_.textures.obstacle);

    float vscale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(config_.textures.suspect.getSize().x);
    float hscale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(config_.textures.suspect.getSize().y);
    config_.sprites.suspect.setScale(vscale, hscale);
    vscale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(config_.textures.staticPlatform.getSize().x);
    config_.sprites.staticPlatform.setScale(vscale, hscale);
    vscale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(config_.textures.mobilePlatform.getSize().x);
    hscale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(config_.textures.mobilePlatform.getSize().y);
    config_.sprites.mobilePlatform.setScale(vscale, hscale);
    vscale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(config_.textures.obstacle.getSize().x);
    hscale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(config_.textures.obstacle.getSize().y);
    config_.sprites.obstacle.setScale(vscale, hscale);
}

void Graphics::render() {
    window_.clear(config_.background.toSFMLColor());
    for (const auto& token : game_->getEnvironment()->getTokens()) {
        if (const Suspect* suspect = dynamic_cast<const Suspect*>(token.second.get()))
            drawObject(config_.sprites.suspect, suspect);
        else if (const StaticPlatform* platform = dynamic_cast<const StaticPlatform*>(token.second.get()))
            drawObject(config_.sprites.staticPlatform, platform);
        else if (const MobilePlatform* platform = dynamic_cast<const MobilePlatform*>(token.second.get()))
            drawObject(config_.sprites.mobilePlatform, platform);
        else if (const Obstacle* obstacle = dynamic_cast<const Obstacle*>(token.second.get()))
            drawObject(config_.sprites.obstacle, obstacle);
    }
    window_.display();
}

void Graphics::drawObject(sf::Sprite& sprite, const Placeholder* object) {
    sprite.setPosition( 
        object->getPosition().x * config_.window.objectSize,
        object->getPosition().y * config_.window.objectSize
    );
    window_.draw(sprite);
}