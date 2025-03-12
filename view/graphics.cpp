#include "graphics.h"

Graphics::Graphics(const std::string& graphicsConfigPath)
    : game_(nullptr), currentMode(Mode::CONFIGURATION), isSimulationPaused(false), isGameTyping(false) {
    graphicsConfig_ = Import::loadGraphicsConfig(graphicsConfigPath);
    window_.create(sf::VideoMode(graphicsConfig_.window.width + 200, graphicsConfig_.window.height), graphicsConfig_.window.title);
    window_.setFramerateLimit(graphicsConfig_.window.frameRateLimit);

    if (!font_.loadFromFile("Arial.ttf")) {
        throw std::runtime_error("Failed to load font for UI elements");
    }
    setupConfigurationUI();
}

void Graphics::adjustGraphicsConfig() {
    graphicsConfig_.window.objectSize = graphicsConfig_.window.height / config_.size.y;

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

    float vscale = 2 * static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.suspect.getSize().y);
    float hscale = 2 * static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.suspect.getSize().x);
    graphicsConfig_.sprites.suspect.setScale(vscale, hscale);
    vscale = 2 * static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.staticPlatform.getSize().y);
    graphicsConfig_.sprites.staticPlatform.setScale(vscale, hscale);
    vscale = 2 * static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.mobilePlatform.getSize().y);
    hscale = 2 * static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.mobilePlatform.getSize().x);
    graphicsConfig_.sprites.mobilePlatform.setScale(vscale, hscale);
    vscale = 2 * static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.obstacle.getSize().y);
    hscale = 2 * static_cast<float>(graphicsConfig_.window.objectSize) / 
            static_cast<float>(graphicsConfig_.textures.obstacle.getSize().x);
    graphicsConfig_.sprites.obstacle.setScale(vscale, hscale);
}

void Graphics::setupConfigurationUI() {
    gameInputBox.setSize(sf::Vector2f(200, 30));
    gameInputBox.setPosition(1000, 160);
    gameInputBox.setFillColor(sf::Color::White);
    gameInputBox.setOutlineThickness(1);
    gameInputBox.setOutlineColor(sf::Color::Black);

    loadButton.setSize(sf::Vector2f(200, 30));
    loadButton.setPosition(1000, 210);
    loadButton.setFillColor(sf::Color::Green);
    
    loadButtonText.setFont(font_);
    loadButtonText.setString("Load Configs");
    loadButtonText.setCharacterSize(15);
    loadButtonText.setPosition(1025, 215);
    loadButtonText.setFillColor(sf::Color::White);

    infoButton.setSize(sf::Vector2f(200, 30));
    infoButton.setPosition(1000, 260);
    infoButton.setFillColor(sf::Color::Blue);
    
    infoButtonText.setFont(font_);
    infoButtonText.setString("Show Info");
    infoButtonText.setCharacterSize(15);
    infoButtonText.setPosition(1025, 265);
    infoButtonText.setFillColor(sf::Color::White);
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
    
    sf::Text gameUserInputText(gameUserInput, font_, 15);
    gameUserInputText.setPosition(1000, 165);
    gameUserInputText.setFillColor(sf::Color::Black);
    window_.draw(gameUserInputText);

    window_.draw(loadButton);
    window_.draw(loadButtonText);
    window_.draw(infoButton);
    window_.draw(infoButtonText);
}

void Graphics::renderPreviewScreen() {
    if (currentMode != Mode::CONFIGURATION)
        return;
    if (graphicsConfig_.window.objectSize) 
        drawGrid();
    for (const auto& token : config_.obstacles) drawObject(graphicsConfig_.sprites.obstacle, token.position);
    for (const auto& token : config_.platforms) {
        if (token.type == PlatformType::STATIC)
            drawObject(graphicsConfig_.sprites.staticPlatform, token.position);
        else drawObject(graphicsConfig_.sprites.mobilePlatform, token.position);
    }
    for (const auto& token : config_.suspects) drawObject(graphicsConfig_.sprites.suspect, token.position);
}

void Graphics::drawObject(sf::Sprite& sprite, Pair object) {
    sprite.setPosition(object.x * graphicsConfig_.window.objectSize - 10, object.y * graphicsConfig_.window.objectSize);
    window_.draw(sprite);
}

void Graphics::startSimulation() {
    currentMode = Mode::SIMULATION;
}

sf::Event Graphics::handleSimulationEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();

        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::P) {
                togglePause();
                return event;
            }
    }
    return event;
}

void Graphics::renderSimulationScreen() {
    drawGrid();
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

void Graphics::toggleEditMode() {
    isEditMode = !isEditMode;
}

void Graphics::handleMouseClickInConfigWindow(const sf::Vector2i& mousePos) {
    if (infoButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        toggleEditMode();
        return;
    }   
    
    if (isEditMode) {
        handleObjectSelection(mousePos);
        return;
    }

    if (gameInputBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        isGameTyping = true;
    } else {
        isGameTyping = false;
    }

    if (loadButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        std::string gameConfigPath = gameUserInput;
        try {
            config_ = Import::loadSystemConfig(gameConfigPath);
        } catch (const std::exception& e) {
            std::cerr << "Error loading config: " << e.what() << std::endl;
            gameUserInput.clear();
            return;
        }
        adjustGraphicsConfig();
        gameUserInput.clear();
    } 
}

void Graphics::handleObjectSelection(const sf::Vector2i& mousePos) {
    int cellX = mousePos.x / graphicsConfig_.window.objectSize;
    int cellY = mousePos.y / graphicsConfig_.window.objectSize;
    if (cellX < 0 || cellX >= config_.size.x || cellY < 0 || cellY >= config_.size.y) {
        toggleEditMode();
        return;
    }

    bool isEmpty = true;
    PlatformConfig placeholderPlatform;
    SuspectConfig placeholderSuspect;

    for (const auto& token : config_.platforms) {
        if (token.position.x == cellX && token.position.y == cellY) {
            isEmpty = false;
            placeholderPlatform = token;
            break;
        }
    }
    for (const auto& token : config_.suspects) {
        if (token.position.x == cellX && token.position.y == cellY) {
            isEmpty = false;
            placeholderSuspect = token;
            break;
        }
    }

    if (isEmpty) return;
    
    std::string objectInfo;
    if (placeholderPlatform.position.x == cellX && placeholderPlatform.position.y == cellY) {
        objectInfo = "Platform\nType: " + std::string(placeholderPlatform.type == PlatformType::STATIC ? "Static" : "Mobile") +
                            "\nPosition: (" + std::to_string(placeholderPlatform.position.x) + ", " + std::to_string(placeholderPlatform.position.y) + ")" +
                            "\nDescription: " + std::string(placeholderPlatform.description.empty() ? "None" : placeholderPlatform.description) +
                            "\nSpeed: " + std::to_string(placeholderPlatform.speed) +
                            "\nMax Energy: " + std::to_string(placeholderPlatform.maxEnergyLevel) +
                            "\nSlot Count: " + std::to_string(placeholderPlatform.slotCount);
        if (!placeholderPlatform.modules.empty()) {
            objectInfo += "\nModules:";
            for (const auto& module : placeholderPlatform.modules) {
                objectInfo += "\n  - Type: " + std::string(module.type == ModuleType::CONNECTION ? "Connection" : module.type == ModuleType::SENSOR ? "Sensor" : "Weapon") +
                            "\n    Energy Consumption: " + std::to_string(module.energyConsumption) +
                            "\n    Range: " + std::to_string(module.range);
                if (module.type == ModuleType::SENSOR)
                    objectInfo += "\n    Sensor Type: " + std::string(module.specific.sensorType == SensorType::Optical ? "Optical" : "XRay");
                if (module.type == ModuleType::WEAPON)
                    objectInfo += "\n    Charging Duration: " + std::to_string(module.specific.chargingDuration.count()) + "ms";
                if (module.type == ModuleType::CONNECTION)
                    objectInfo += "\n    Max Sessions: " + std::to_string(module.specific.maxSessions);
            }
        }
    }
    if (placeholderSuspect.position.x == cellX && placeholderSuspect.position.y == cellY) {
        objectInfo = "Suspect\nPosition: (" + std::to_string(placeholderSuspect.position.x) + ", " + std::to_string(placeholderSuspect.position.y) + ")" +
                            "\nSensor Range: " + std::to_string(placeholderSuspect.sensorRange) +
                            "\nSpeed: " + std::to_string(placeholderSuspect.speed);
    }

    int lineCount = std::count(objectInfo.begin(), objectInfo.end(), '\n') + 1;
    int lineHeight = 17;
    int windowHeight = lineCount * lineHeight + 20;

    sf::RenderWindow popup;
    popup.create(sf::VideoMode(250, windowHeight), "Objects Info");
    popup.clear(sf::Color::White);

    sf::Text infoText(objectInfo, font_, 15);
    infoText.setFillColor(sf::Color::Black);

    while (popup.isOpen()) {
        sf::Event event;
        while (popup.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                popup.close();
            }
        }

        popup.clear(sf::Color::White);
        popup.draw(infoText);
        popup.display();
    }
}

void Graphics::drawGrid() {
    sf::VertexArray lines(sf::Lines);
    for (int i = 0; i <= graphicsConfig_.window.width / graphicsConfig_.window.objectSize; ++i) {
        lines.append(sf::Vertex(sf::Vector2f(i * graphicsConfig_.window.objectSize, 0), sf::Color::Black));
        lines.append(sf::Vertex(sf::Vector2f(i * graphicsConfig_.window.objectSize, config_.size.y * graphicsConfig_.window.objectSize), sf::Color::Black));
    }
    for (int j = 0; j <= graphicsConfig_.window.height / graphicsConfig_.window.objectSize; ++j) {
        lines.append(sf::Vertex(sf::Vector2f(0, j * graphicsConfig_.window.objectSize), sf::Color::Black));
        lines.append(sf::Vertex(sf::Vector2f(config_.size.x * graphicsConfig_.window.objectSize, j * graphicsConfig_.window.objectSize), sf::Color::Black));
    }
    window_.draw(lines);
}