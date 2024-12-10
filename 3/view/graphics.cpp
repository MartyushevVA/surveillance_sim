#include "graphics.h"

#include "../model/system/environment.h"

void Graphics::handleEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();
    }
}

void Graphics::render() {
    window_.clear(config_.background.toSFMLColor());
    for (const auto& token : game_->getEnvironment()->getTokens()) {
        if (const Suspect* suspect = dynamic_cast<const Suspect*>(token.second.get()))
            drawObject(sprites_.suspect, suspect);
        else if (const StaticPlatform* platform = dynamic_cast<const StaticPlatform*>(token.second.get()))
            drawObject(sprites_.staticPlatform, platform);
        else if (const MobilePlatform* platform = dynamic_cast<const MobilePlatform*>(token.second.get()))
            drawObject(sprites_.mobilePlatform, platform);
        else if (const Obstacle* obstacle = dynamic_cast<const Obstacle*>(token.second.get()))
            drawObject(sprites_.obstacle, obstacle);
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