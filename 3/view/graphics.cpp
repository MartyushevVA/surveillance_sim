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
    config_.window.pixelRatio = j["window"]["pixel_ratio"];

    auto loadColor = [](const nlohmann::json& j) -> GraphicsConfig::Color {
        return {
            static_cast<uint8_t>(j["r"]),
            static_cast<uint8_t>(j["g"]),
            static_cast<uint8_t>(j["b"])
        };
    };

    config_.colors.background = loadColor(j["colors"]["background"]);
    config_.colors.suspect = loadColor(j["colors"]["suspect"]);
    config_.colors.staticPlatform = loadColor(j["colors"]["static_platform"]);
    config_.colors.mobilePlatform = loadColor(j["colors"]["mobile_platform"]);
    config_.colors.obstacle = loadColor(j["colors"]["obstacle"]);
}

void Graphics::handleEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();
    }
}

void Graphics::render(const Environment& environment) {
    window_.clear(config_.colors.background.toSFMLColor());
    
    for (const auto& token : environment.getTokens()) {
        if (const Suspect* suspect = dynamic_cast<const Suspect*>(token.second.get()))
            drawSuspect(suspect);
        else if (const StaticPlatform* platform = dynamic_cast<const StaticPlatform*>(token.second.get()))
            drawStaticPlatform(platform);
        else if (const MobilePlatform* platform = dynamic_cast<const MobilePlatform*>(token.second.get()))
            drawMobilePlatform(platform);
        else if (const Obstacle* obstacle = dynamic_cast<const Obstacle*>(token.second.get()))
            drawObstacle(obstacle);
    }
    
    window_.display();
}

void Graphics::drawSuspect(const Suspect* suspect) {
    sf::CircleShape shape(static_cast<float>(config_.window.pixelRatio) / 2.f);
    shape.setFillColor(config_.colors.suspect.toSFMLColor());
    shape.setPosition(
        suspect->getPosition().x * config_.window.pixelRatio,
        suspect->getPosition().y * config_.window.pixelRatio
    );
    window_.draw(shape);
}

void Graphics::drawStaticPlatform(const StaticPlatform* platform) {
    sf::RectangleShape shape(sf::Vector2f(
        static_cast<float>(config_.window.pixelRatio),
        static_cast<float>(config_.window.pixelRatio)
    ));
    shape.setFillColor(config_.colors.staticPlatform.toSFMLColor());
    shape.setPosition(
        platform->getPosition().x * config_.window.pixelRatio,
        platform->getPosition().y * config_.window.pixelRatio
    );
    window_.draw(shape);
}

void Graphics::drawMobilePlatform(const MobilePlatform* platform) {
    sf::RectangleShape shape(sf::Vector2f(
        static_cast<float>(config_.window.pixelRatio),
        static_cast<float>(config_.window.pixelRatio)
    ));
    shape.setFillColor(config_.colors.mobilePlatform.toSFMLColor());
    shape.setPosition(
        platform->getPosition().x * config_.window.pixelRatio,
        platform->getPosition().y * config_.window.pixelRatio
    );
    window_.draw(shape);
}

void Graphics::drawObstacle(const Obstacle* obstacle) {
    sf::RectangleShape shape(sf::Vector2f(
        static_cast<float>(config_.window.pixelRatio),
        static_cast<float>(config_.window.pixelRatio)
    ));
    shape.setFillColor(config_.colors.obstacle.toSFMLColor());
    shape.setPosition(
        obstacle->getPosition().x * config_.window.pixelRatio,
        obstacle->getPosition().y * config_.window.pixelRatio
    );
    window_.draw(shape);
}