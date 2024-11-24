#include <iostream>

#include "game.h"

#include "mobile_platform.h"
#include "static_platform.h"
#include "suspect.h"
#include "module_types.h"

void Game::start() {
    window_.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Surveillance Game");
    window_.setFramerateLimit(60);

    while (window_.isOpen()) {
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
            sf::CircleShape shape(static_cast<float>(RATIO) / 2.f);
            shape.setFillColor(sf::Color::Red);
            shape.setPosition(suspect->getPosition().x * RATIO, suspect->getPosition().y * RATIO);
            window_.draw(shape);
        }
        if (StaticPlatform* platform = dynamic_cast<StaticPlatform*>(token.get())) {
            sf::RectangleShape shape(sf::Vector2f(static_cast<float>(RATIO), static_cast<float>(RATIO)));
            shape.setFillColor(sf::Color::White);
            shape.setPosition(platform->getPosition().x * RATIO, platform->getPosition().y * RATIO);
            window_.draw(shape);
        }
        if (MobilePlatform* platform = dynamic_cast<MobilePlatform*>(token.get())) {
            sf::RectangleShape shape(sf::Vector2f(static_cast<float>(RATIO), static_cast<float>(RATIO)));
            shape.setFillColor(sf::Color::Blue);
            shape.setPosition(platform->getPosition().x * RATIO, platform->getPosition().y * RATIO);
            window_.draw(shape);
        }
        if (Obstacle* obstacle = dynamic_cast<Obstacle*>(token.get())) {
            sf::RectangleShape shape(sf::Vector2f(static_cast<float>(RATIO), static_cast<float>(RATIO)));
            shape.setFillColor(sf::Color::Magenta);
            shape.setPosition(obstacle->getPosition().x * RATIO, obstacle->getPosition().y * RATIO);
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
                if (Pair attackableSuspect = weapon->findAttackableSuspect(report); attackableSuspect != Pair{-1, 0}) {
                    std::cout << "Attackable suspect found" << std::endl;
                    weapon->attack(attackableSuspect);
                    std::cout << "Attacked suspect at " << attackableSuspect.x << ", " << attackableSuspect.y << std::endl;
                }
            else if (MobilePlatform* officer = dynamic_cast<MobilePlatform*>(platform.get()))
                if (Pair pursuitableSuspect = officer->findPursuitableSuspect(report); pursuitableSuspect != Pair{-1, 0})
                    platform->move(officer->calculatePursuitMove(pursuitableSuspect));
                else
                    platform->move(officer->calculateRandomMove());
       }
    }
}