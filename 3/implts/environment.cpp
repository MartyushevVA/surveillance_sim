#include "environment.h"

#include "platform.h"
#include "suspect.h"

void Environment::addToken(std::shared_ptr<Placeholder> token) {
    if (token->getPosition().x >= size_.x || token->getPosition().y >= size_.y)
        throw std::invalid_argument("Token position is out of bounds");
    if (getCellType(token->getPosition()) != CellType::Empty)
        throw std::invalid_argument("Cell is already occupied");
    tokens_.insert(token);
}

void Environment::removeToken(std::shared_ptr<Placeholder> token) {
    tokens_.erase(token);
}

void Environment::removeToken(Pair position) {
    tokens_.erase(getToken(position));
}

std::shared_ptr<Placeholder> Environment::getToken(Pair position) const {
    for (const auto& token : tokens_)
        if (token->getPosition().x == position.x && token->getPosition().y == position.y)
            return token;
    return nullptr;
}

CellType Environment::getCellType(Pair position) const {
    for (const auto& token : tokens_) {
        if (token->getPosition().x == position.x && token->getPosition().y == position.y) {
            if (dynamic_cast<Obstacle*>(token.get())) return CellType::Obstacle;
            if (dynamic_cast<Platform*>(token.get())) return CellType::Platform;
            if (dynamic_cast<Suspect*>(token.get())) return CellType::Suspect;
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
        if (getCellType({x, y}) == CellType::Obstacle)
            return false;
    }
    return true;
}

double Environment::howFar(Pair from, Pair to, int range) const {
    return sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y)) / range;
}