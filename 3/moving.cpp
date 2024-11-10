#include "moving_object.h"
#include "mobile_platform.h"
#include "intruder.h"
#include "environment.h"
#include <random>

void MovingObject::move(Pair nextPosition) {
    if (abilityToMove(nextPosition)) {
        setPosition(nextPosition.x, nextPosition.y);
    }
}

bool MovingObject::abilityToMove(Pair position) const {
    if (!environment_) return false;
    
    Pair size = environment_->getSize();
    if (position.x < 0 || position.x >= size.x || 
        position.y < 0 || position.y >= size.y) {
        return false;
    }

    return environment_->getCellType(position.x, position.y) == CellType::Empty;
}

Pair MovingObject::calculateRandomMove() const {
    srand(time(nullptr));
    int spd = speed_; //it depends on angle
    int dx = ((rand() % 3) - 1) * spd;
    int dy = ((rand() % 3) - 1) * spd;
    Pair currentPos = getPosition();
    return {currentPos.x + dx, currentPos.y + dy};
}

Pair MobilePlatform::calculatePursuitMove(Pair target) const {
    Pair currentPos = Platform::getPosition();
    int spd = speed_; //it depends on angle
    int dx = target.x - currentPos.x;
    int dy = target.y - currentPos.y;
    dx = (dx != 0) ? (dx / abs(dx)) * spd : 0;
    dy = (dy != 0) ? (dy / abs(dy)) * spd : 0;
    return {currentPos.x + dx, currentPos.y + dy};
}

Pair Intruder::calculateAvoidanceMove(Pair threat) const {
    Pair currentPos = getPosition();
    int spd = speed_; //it depends on angle
    int dx = threat.x - currentPos.x;
    int dy = threat.y - currentPos.y;
    dx = (dx != 0) ? (dx / abs(dx)) * spd : 0;
    dy = (dy != 0) ? (dy / abs(dy)) * spd : 0;
    return {currentPos.x + dx, currentPos.y + dy};
}