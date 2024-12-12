#include "objects.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <algorithm>

void MovablePlaceholder::move(Pair position) {
    environment_.lock()->moveToken(position_, position);
}

Pair Suspect::opponentBasedMove(Pair opponent) const {
    auto environmentPtr = environment_.lock();
    if (!environmentPtr) return position_;
    double dx = opponent.x - position_.x;
    double dy = opponent.y - position_.y;
    double distance = environmentPtr->calculateDistance(position_, opponent);
    double nx = -dx / distance;
    double ny = -dy / distance;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angle_dist(-0.1, 0.1);
    double angle_adjustment = angle_dist(gen);

    double cos_angle = cos(angle_adjustment);
    double sin_angle = sin(angle_adjustment);
    double adjusted_nx = nx * cos_angle - ny * sin_angle;
    double adjusted_ny = nx * sin_angle + ny * cos_angle;

    int dx_move = std::clamp(static_cast<int>(adjusted_nx * speed_), -speed_, speed_);
    int dy_move = std::clamp(static_cast<int>(adjusted_ny * speed_), -speed_, speed_);
    Pair newPos = {
        position_.x + dx_move,
        position_.y + dy_move
    };
    if (environmentPtr->isValidCoords(newPos))
        return newPos;
    Pair altPos1 = {position_.x + dx_move, position_.y};
    if (environmentPtr->isValidCoords(altPos1))
        return altPos1;
    Pair altPos2 = {position_.x, position_.y + dy_move};
    if (environmentPtr->isValidCoords(altPos2))
        return altPos2;
    return position_;
}

Pair MovablePlaceholder::randomMove() const {
    auto environmentPtr = environment_.lock();
    if (!environmentPtr) return position_;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> angle_dist(0, 2 * M_PI);

    double angle = angle_dist(gen);
    double nx = cos(angle);
    double ny = sin(angle);
    int dx_move = std::clamp(static_cast<int>(nx * speed_), -speed_, speed_);
    int dy_move = std::clamp(static_cast<int>(ny * speed_), -speed_, speed_);   
    Pair newPos = {
        position_.x + dx_move,
        position_.y + dy_move
    };
    if (environmentPtr->isValidCoords(newPos))
        return newPos;
    return position_;
}

Pair MobilePlatform::opponentBasedMove(Pair opponent) const {
    auto environmentPtr = environment_.lock();
    if (!environmentPtr) return position_;
    double dx = opponent.x - position_.x;
    double dy = opponent.y - position_.y;
    double distance = environmentPtr->calculateDistance(position_, opponent);
    double nx = dx / distance;
    double ny = dy / distance;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angle_dist(-0.1, 0.1);
    double angle_adjustment = angle_dist(gen);
    double cos_angle = cos(angle_adjustment);
    double sin_angle = sin(angle_adjustment);
    double adjusted_nx = nx * cos_angle - ny * sin_angle;
    double adjusted_ny = nx * sin_angle + ny * cos_angle;

    int dx_move = std::clamp(static_cast<int>(adjusted_nx * speed_), -speed_, speed_);
    int dy_move = std::clamp(static_cast<int>(adjusted_ny * speed_), -speed_, speed_);
    Pair newPos = {
        position_.x + dx_move,
        position_.y + dy_move
    };
    if (environmentPtr->isValidCoords(newPos))
        return newPos;
    Pair altPos1 = {position_.x + dx_move, position_.y};
    if (environmentPtr->isValidCoords(altPos1))
        return altPos1;  
    Pair altPos2 = {position_.x, position_.y + dy_move};
    if (environmentPtr->isValidCoords(altPos2))
        return altPos2;
    return position_;
}