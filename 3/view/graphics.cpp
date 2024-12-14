#include "graphics.h"

Graphics::Graphics(const std::string& graphicsConfigPath)
    : game_(nullptr), currentMode(Mode::CONFIGURATION), isSimulationPaused(false), isGameTyping(false) {
    adjustGraphicsConfig(Import::loadGraphicsConfig(graphicsConfigPath));
    window_.create(sf::VideoMode(graphicsConfig_.window.width, graphicsConfig_.window.height), graphicsConfig_.window.title);
    window_.setFramerateLimit(graphicsConfig_.window.frameRateLimit);

    if (!font_.loadFromFile("Arial.ttf")) {
        throw std::runtime_error("Failed to load font for UI elements");
    }
    setupConfigurationUI();
}

void Graphics::adjustGraphicsConfig(GraphicsConfig config) {
    graphicsConfig_ = config;

    auto loadTexture = [](const std::string& path) -> sf::Texture {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        return texture;
    };

    graphicsConfig_.textures.suspect = loadTexture(graphicsConfig_.sources.suspectNameOfFile);
    graphicsConfig_.textures.staticPlatform = loadTexture(graphicsConfig_.sources.staticPlatformNameOfFile);
    graphicsConfig_.textures.mobilePlatform = loadTexture(graphicsConfig_.sources.mobilePlatformNameOfFile);
    graphicsConfig_.textures.obstacle = loadTexture(graphicsConfig_.sources.obstacleNameOfFile);

    graphicsConfig_.sprites.suspect.setTexture(graphicsConfig_.textures.suspect);
    graphicsConfig_.sprites.staticPlatform.setTexture(graphicsConfig_.textures.staticPlatform);
    graphicsConfig_.sprites.mobilePlatform.setTexture(graphicsConfig_.textures.mobilePlatform);
    graphicsConfig_.sprites.obstacle.setTexture(graphicsConfig_.textures.obstacle);

    float vscale = static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.suspect.getSize().y);
    float hscale = static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.suspect.getSize().x);
    graphicsConfig_.sprites.suspect.setScale(vscale, hscale);
    vscale = static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.staticPlatform.getSize().y);
    graphicsConfig_.sprites.staticPlatform.setScale(vscale, hscale);
    vscale = static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.mobilePlatform.getSize().y);
    hscale = static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.mobilePlatform.getSize().x);
    graphicsConfig_.sprites.mobilePlatform.setScale(vscale, hscale);
    vscale = static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.obstacle.getSize().y);
    hscale = static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.obstacle.getSize().x);
    graphicsConfig_.sprites.obstacle.setScale(vscale, hscale);
}

void Graphics::setupConfigurationUI() {
    gameInputLabel.setFont(font_);
    gameInputLabel.setString("Enter Game Config File Path:");
    gameInputLabel.setCharacterSize(20);
    gameInputLabel.setPosition(50, 130);

    gameInputBox.setSize(sf::Vector2f(400, 30));
    gameInputBox.setPosition(50, 160);
    gameInputBox.setFillColor(sf::Color::White);
    gameInputBox.setOutlineThickness(1);
    gameInputBox.setOutlineColor(sf::Color::Black);

    loadButton.setSize(sf::Vector2f(200, 50));
    loadButton.setPosition(50, 210);
    loadButton.setFillColor(sf::Color::Green);
    
    loadButtonText.setFont(font_);
    loadButtonText.setString("Load Configs");
    loadButtonText.setCharacterSize(20);
    loadButtonText.setPosition(75, 225);
    loadButtonText.setFillColor(sf::Color::White);

    manualButton.setSize(sf::Vector2f(200, 50));
    manualButton.setPosition(300, 210);
    manualButton.setFillColor(sf::Color::Blue);
    
    manualButtonText.setFont(font_);
    manualButtonText.setString("Manual Config");
    manualButtonText.setCharacterSize(20);
    manualButtonText.setPosition(325, 225);
    manualButtonText.setFillColor(sf::Color::White);
}

void Graphics::togglePause() {
    isSimulationPaused = !isSimulationPaused;
}

void Graphics::renderConfigurationUI() {
    if (currentMode != Mode::CONFIGURATION) {
        return;
    }
    window_.draw(gameInputLabel);
    window_.draw(gameInputBox);
    
    sf::Text gameUserInputText(gameUserInput, font_, 20);
    gameUserInputText.setPosition(55, 165);
    gameUserInputText.setFillColor(sf::Color::Black);
    window_.draw(gameUserInputText);

    window_.draw(loadButton);
    window_.draw(loadButtonText);
    window_.draw(manualButton);
    window_.draw(manualButtonText);
}

void Graphics::renderPreviewScreen() {
    if (currentMode != Mode::CONFIGURATION) {
        return;
    }
    for (const auto& token : config_.obstacles) drawObject(graphicsConfig_.sprites.obstacle, token.position);
    for (const auto& token : config_.platforms) {
        if (token.type == PlatformType::STATIC)
            drawObject(graphicsConfig_.sprites.staticPlatform, token.position);
        else drawObject(graphicsConfig_.sprites.mobilePlatform, token.position);
    }
    for (const auto& token : config_.suspects) drawObject(graphicsConfig_.sprites.suspect, token.position);
}

void Graphics::drawObject(sf::Sprite& sprite, Pair object) {
    sprite.setPosition(object.x * graphicsConfig_.window.objectSize, object.y * graphicsConfig_.window.objectSize);
    window_.draw(sprite);
}

void Graphics::startSimulation() {
    currentMode = Mode::SIMULATION;
}

void Graphics::handleSimulationEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();

        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::P)
                togglePause();
    }
}

void Graphics::renderSimulationScreen() {
    for (const auto& token : game_->getEnvironment()->getTokens()) {
        if (const Suspect* suspect = dynamic_cast<const Suspect*>(token.second.get()))
            drawObject(graphicsConfig_.sprites.suspect, suspect->getPosition());
        else if (const StaticPlatform* platform = dynamic_cast<const StaticPlatform*>(token.second.get()))
            drawObject(graphicsConfig_.sprites.staticPlatform, platform->getPosition());
        else if (const MobilePlatform* platform = dynamic_cast<const MobilePlatform*>(token.second.get()))
            drawObject(graphicsConfig_.sprites.mobilePlatform, platform->getPosition());
        else if (const Obstacle* obstacle = dynamic_cast<const Obstacle*>(token.second.get()))
            drawObject(graphicsConfig_.sprites.obstacle, obstacle->getPosition());
    }
    window_.display();
    window_.clear(graphicsConfig_.background.toSFMLColor());
}

SystemConfig Graphics::runConfigurationWindow() {
    window_.setTitle("Configuration");
    window_.setFramerateLimit(60);
    window_.setVerticalSyncEnabled(true);

    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
                return SystemConfig();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
                handleMouseClickInConfigWindow(mousePos);
            }

            if (isGameTyping) {
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode < 128) {
                        if (event.text.unicode == '\b') {
                            if (!gameUserInput.empty()) {
                                gameUserInput.pop_back();
                            }
                        } else {
                            gameUserInput += static_cast<char>(event.text.unicode);
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                return config_;
            }
        }

        renderPreviewScreen();
        renderConfigurationUI();
        
        window_.display();
        window_.clear(graphicsConfig_.background.toSFMLColor());
    }
    return config_;
}

void Graphics::handleMouseClickInConfigWindow(const sf::Vector2i& mousePos) {
    if (gameInputBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        isGameTyping = true;
    } else {
        isGameTyping = false;
    }

    if (loadButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        std::string gameConfigPath = gameUserInput;
        config_ = Import::loadSystemConfig(gameConfigPath);
        gameUserInput.clear();
    }

    if (manualButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        handleObjectSelection(mousePos);
    }
}

void Graphics::handleObjectSelection(const sf::Vector2i& mousePos) {
    // Logic to add or remove objects based on the mouse position in the preview area
    // For example, check if the click is within the bounds of an object and toggle its state
}