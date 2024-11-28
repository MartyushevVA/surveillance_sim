#include "environment.h"

#include "platform.h"
#include "suspect.h"

void Environment::addToken(std::shared_ptr<Placeholder> token) {
    if (token->getPosition().x >= size_.x || token->getPosition().y >= size_.y)
        throw std::invalid_argument("Token position is out of bounds");
    if (!isEmpty(token->getPosition()))
        throw std::invalid_argument("Cell is already occupied");
    tokens_.insert({token->getPosition(), token});
}

void Environment::removeToken(Pair position) {
    auto token = getToken(position);
    tokens_.erase(position);
}

std::shared_ptr<Placeholder> Environment::getToken(Pair position) const {
    auto token = tokens_.find(position);
    return token != tokens_.end() ? token->second : nullptr;
}

bool Environment::abilityToMove(Pair from, Pair to) const {
    if (to.x >= size_.x || to.y >= size_.y || to.x < 0 || to.y < 0)
        return false;
    auto token = getToken(from);
    std::cout << "Token at " << from.x << ", " << from.y << " is " << (token ? "not null" : "null") << std::endl;
    if (!token) return false;
    if (from != to && !isEmpty(to)) {
        std::cout << "Cell at " << to.x << ", " << to.y << " is occupied" << std::endl;
        return false;
    }
    if (!hasLineOfSight(from, to)) {
        std::cout << "No line of sight from " << from.x << ", " << from.y << " to " << to.x << ", " << to.y << std::endl;
        return false;
    }
    return true;
}

void Environment::moveToken(Pair from, Pair to) {
    auto token = getToken(from);
    tokens_.erase(from);
    tokens_.insert({to, token});
    token->setPosition(to);
}

bool Environment::isEmpty(Pair position) const {
    return getToken(position) == nullptr;
}

bool Environment::hasLineOfSight(Pair from, Pair to) const {
    double distance = sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
    double stepX = (to.x - from.x) / distance;
    double stepY = (to.y - from.y) / distance;

    for (double i = 1; i < distance; i += 1) {
        int x = from.x + stepX * i;
        int y = from.y + stepY * i;
        if (!isEmpty({x, y}))
            return false;
    }
    return true;
}

double Environment::howFar(Pair from, Pair to, int range) const {
    return getDistance(from, to) / range;
}

int Environment::getDistance(Pair from, Pair to) const {
    return abs(from.x - to.x) + abs(from.y - to.y);
}