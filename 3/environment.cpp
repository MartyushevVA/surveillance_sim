#include "environment.h"
#include "intruder.h"
#include "platform.h"

void Environment::addToken(std::shared_ptr<Placeholder> token) {
    if (token->getPosition().x < size_.x && token->getPosition().y < size_.y) {
        tokens_.insert(token);
    } else {
        throw std::invalid_argument("Token position is out of bounds");
    }
}

std::shared_ptr<Placeholder> Environment::removeToken(int x, int y) {
    if (auto it = tokens_.find(std::make_shared<Placeholder>(Pair{x, y})); it != tokens_.end()) {
        auto token = std::move(*it);
        tokens_.erase(it);
        return token;
    }
    return nullptr;
}

std::set<std::shared_ptr<Placeholder>> Environment::getTokens() const {
    return tokens_;
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

void Environment::setCellType(int x, int y, CellType type) {
    auto it = tokens_.find(std::make_shared<Placeholder>(Pair{x, y}));
    if (it != tokens_.end()) {
        tokens_.erase(it);
    }

    switch (type) {
        case CellType::Obstacle:
            addToken(std::make_shared<Obstacle>(Pair{x, y}, this));
            break;
        case CellType::Platform:
            addToken(std::make_shared<Platform>(Pair{x, y}, this));
            break;
        case CellType::Intruder:
            addToken(std::make_shared<Intruder>(Pair{x, y}, this, 1));
            break;
    }
}