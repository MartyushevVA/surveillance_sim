#include "graphics.h"

Graphics::Graphics(const std::string& configPath) {
    loadConfig(configPath);
    window_.create(
        sf::VideoMode(config_.window.width, config_.window.height),
        config_.window.title
    );
    window_.setFramerateLimit(config_.window.frameRateLimit);
}

void Graphics::loadConfig(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open graphics config file: " + configPath);
    }

    nlohmann::json j;
    file >> j;

    config_.window.width = j["window"]["width"];
    config_.window.height = j["window"]["height"];
    config_.window.title = j["window"]["title"];
    config_.window.frameRateLimit = j["window"]["framerate_limit"];
    config_.window.objectSize = j["window"]["object_size"];

    auto loadColor = [](const nlohmann::json& j) -> GraphicsConfig::Color {
        return {
            static_cast<uint8_t>(j["r"]),
            static_cast<uint8_t>(j["g"]),
            static_cast<uint8_t>(j["b"])
        };
    };

    config_.background = loadColor(j["background_color"]);

    auto loadTexture = [](const std::string& path) -> sf::Texture {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        return texture;
    };

    textures_.suspect = loadTexture(j["textures"]["suspect"]);
    textures_.staticPlatform = loadTexture(j["textures"]["static_platform"]);
    textures_.mobilePlatform = loadTexture(j["textures"]["mobile_platform"]);
    textures_.obstacle = loadTexture(j["textures"]["obstacle"]);

    sprites_.suspect.setTexture(textures_.suspect);
    sprites_.staticPlatform.setTexture(textures_.staticPlatform);
    sprites_.mobilePlatform.setTexture(textures_.mobilePlatform);
    sprites_.obstacle.setTexture(textures_.obstacle);

    float scale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(textures_.suspect.getSize().x);
    sprites_.suspect.setScale(scale, scale);
    scale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(textures_.staticPlatform.getSize().x);
    sprites_.staticPlatform.setScale(scale, scale);
    scale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(textures_.mobilePlatform.getSize().x);
    sprites_.mobilePlatform.setScale(scale, scale);
    scale = static_cast<float>(config_.window.objectSize) / 
            static_cast<float>(textures_.obstacle.getSize().x);
    sprites_.obstacle.setScale(scale, scale);
}

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