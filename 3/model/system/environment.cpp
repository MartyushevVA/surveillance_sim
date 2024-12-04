#include "environment.h"

#include <cmath>

#include "../objects/objects.h"

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
    if (!token) return false;
    if (from != to && !isEmpty(to))
        return false;
    if (!hasLineOfSight(from, to))
        return false;
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
    double distance = getDistance(from, to);
    double stepX = (to.x - from.x) / distance;
    double stepY = (to.y - from.y) / distance;

    for (int i = 0; i <= distance; i++) {
        int x = static_cast<int>(from.x + stepX * i);
        int y = static_cast<int>(from.y + stepY * i);
        if (Pair{x, y} == from || Pair{x, y} == to)
            continue;
        if (!isEmpty({x, y}))
            return false;
    }
    return true;
}

double Environment::howFar(Pair from, Pair to, int range) const {
    return getDistance(from, to) / range;
}

double Environment::getDistance(Pair from, Pair to) const {
    return sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
}
