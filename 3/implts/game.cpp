#include "game.h"

#include "mobile_platform.h"
#include "suspect.h"
#include "module_types.h"

void Game::start() {
    window_.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Surveillance Game");
    window_.setFramerateLimit(60);

    bool isRunning = true;
    while (isRunning && window_.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window_.pollEvent(event))
        if (event.type == sf::Event::Closed)
            window_.close();
}

void Game::render() {
    window_.clear(sf::Color::Black);
    
    for (const auto& token : environment_.getTokens()) {
        if (Suspect* suspect = dynamic_cast<Suspect*>(token.get())) {
            sf::CircleShape shape(10.f);
            shape.setFillColor(sf::Color::Red);
            shape.setPosition(suspect->getPosition().x, suspect->getPosition().y);
            window_.draw(shape);
        }
        if (Platform* platform = dynamic_cast<Platform*>(token.get())) {
            sf::RectangleShape shape(sf::Vector2f(20.f, 20.f));
            shape.setFillColor(sf::Color::Blue);
            shape.setPosition(platform->getPosition().x, platform->getPosition().y);
            window_.draw(shape);
        }
    }
    window_.display();
}

void Game::update() {
    for (const auto& token : environment_.getTokens())
        if (Suspect* suspect = dynamic_cast<Suspect*>(token.get())) {
            if (Platform* predator = suspect->nearestPredatorWithinRange())
                token->move(suspect->calculateAvoidanceMove(predator->getPosition()));
            else
                token->move(suspect->calculateRandomMove());
        }

    for (const auto& platform : ai_.getConnectedPlatforms()) {
        if (SensorModule* sensor = platform->findModuleOfType<SensorModule>()) {
            Report report = sensor->getSurrounding();
            if (WeaponModule* weapon = platform->findModuleOfType<WeaponModule>())
                if (Pair attackableSuspect = weapon->findAttackableSuspect(report); attackableSuspect != Pair{-1, 0})
                    weapon->attack(attackableSuspect);
            else if (MobilePlatform* officer = dynamic_cast<MobilePlatform*>(platform.get()))
                if (Pair pursuitableSuspect = officer->findPursuitableSuspect(report); pursuitableSuspect != Pair{-1, 0})
                    platform->move(officer->calculatePursuitMove(pursuitableSuspect));
                else
                    platform->move(officer->calculateRandomMove());
       }
    }
}