#include "environment.h"

void Environment::addToken(std::shared_ptr<Placeholder> token) {
    if (token->getPosition().x >= size_.x || token->getPosition().y >= size_.y)
        throw std::invalid_argument("Token position is out of bounds");
    if (getCellType(token->getPosition().x, token->getPosition().y) != CellType::Empty)
        throw std::invalid_argument("Cell is already occupied");
    tokens_.insert(token);
}

std::shared_ptr<Placeholder> Environment::extractToken(int x, int y) {
    for (const auto& token : tokens_) {
        if (token->getPosition().x == x && token->getPosition().y == y) {
            auto extractedToken = std::move(token);
            tokens_.erase(token);
            return extractedToken;
        }
    }
    return nullptr;
}

std::shared_ptr<Placeholder> Environment::getToken(int x, int y) const {
    for (const auto& token : tokens_)
        if (token->getPosition().x == x && token->getPosition().y == y)
            return token;
    return nullptr;
}

CellType Environment::getCellType(int x, int y) const {
    for (const auto& token : tokens_) {
        if (token->getPosition().x == x && token->getPosition().y == y) {
            if (dynamic_cast<Obstacle*>(token.get())) return CellType::Obstacle;
            if (dynamic_cast<Platform*>(token.get())) return CellType::Platform;
            if (dynamic_cast<Intruder*>(token.get())) return CellType::Intruder;
        }
    }
    return CellType::Empty;
}

bool Environment::hasLineOfSight(Pair from, Pair to) const {
    double distance = sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
    double stepX = (to.x - from.x) / distance;
    double stepY = (to.y - from.y) / distance;

    for (double i = 0; i < distance; i += 1) {
        int x = from.x + stepX * i;
        int y = from.y + stepY * i;
        if (getCellType(x, y) != CellType::Obstacle)
            return false;
    }
    return true;
}