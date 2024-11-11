#include "moving_object.h"
#include "mobile_platform.h"
#include "intruder.h"
#include "environment.h"
#include <random>

Pair MobilePlatform::calculateRandomMove() const {
    srand(time(nullptr));
    int spd = speed_; //it depends on angle
    int dx = ((rand() % 3) - 1) * spd;
    int dy = ((rand() % 3) - 1) * spd;
    return {position_.x + dx, position_.y + dy};
}

Pair Intruder::calculateRandomMove() const {
    srand(time(nullptr));
    int spd = speed_; //it depends on angle
    int dx = ((rand() % 3) - 1) * spd;
    int dy = ((rand() % 3) - 1) * spd;
    return {position_.x + dx, position_.y + dy};
}

Pair MobilePlatform::calculatePursuitMove(Pair target) const {
    int spd = speed_; //it depends on angle
    int dx = target.x - position_.x;
    int dy = target.y - position_.y;
    dx = (dx != 0) ? (dx / abs(dx)) * spd : 0;
    dy = (dy != 0) ? (dy / abs(dy)) * spd : 0;
    return {position_.x + dx, position_.y + dy};
}

Pair Intruder::calculateAvoidanceMove(Pair threat) const {
    int spd = speed_; //it depends on angle
    int dx = threat.x - position_.x;
    int dy = threat.y - position_.y;
    dx = (dx != 0) ? (dx / abs(dx)) * spd : 0;
    dy = (dy != 0) ? (dy / abs(dy)) * spd : 0;
    return {position_.x - dx, position_.y - dy};
}