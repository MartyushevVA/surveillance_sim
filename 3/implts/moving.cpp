#include <random>

#include <iostream>

#include "placeholder.h"
#include "mobile_platform.h"
#include "suspect.h"

#include "environment.h"

void Placeholder::move(Pair position) {
    if (environment_->abilityToMove(position_, position))
        environment_->moveToken(position_, position);
    update();
}

Pair Placeholder::calculateRandomMove() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    for (int i = 0; i < 8; i++) {
        std::uniform_int_distribution<> dis(-speed_, speed_);
        int dx = dis(gen);
        int remaining = speed_ * speed_ - dx * dx;
        int dy = 0;
        if (remaining > 0) {
            int max_dy = static_cast<int>(sqrt(remaining));
            std::uniform_int_distribution<> dis_y(-max_dy, max_dy);
            dy = dis_y(gen);
        }
        Pair newPos = {position_.x + dx, position_.y + dy};
        if (environment_->abilityToMove(position_, newPos))
            return newPos;
    }
    return position_;
}

Pair MobilePlatform::calculatePursuitMove(Pair target) const {
    double dx = target.x - position_.x;
    double dy = target.y - position_.y;
    double distance = sqrt(dx * dx + dy * dy);
    int speed = distance > speed_ ? speed_ : static_cast<int>(distance) - 1;
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

    double adjusted_dx = adjusted_nx * speed;
    double adjusted_dy = adjusted_ny * speed;

    return {position_.x + static_cast<int>(adjusted_dx), position_.y + static_cast<int>(adjusted_dy)};
}

Pair Suspect::calculateAvoidanceMove(Pair threat) const {
    double dx = threat.x - position_.x;
    double dy = threat.y - position_.y;
    double distance = sqrt(dx * dx + dy * dy);
    double nx = dx / distance;
    double ny = dy / distance;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angle_dist(0, 6.283185307179586);
    for (int i = 0; i < 8; i++) {
        double angle = angle_dist(gen);
        double move_x = cos(angle);
        double move_y = sin(angle);
        Pair newPos = {
            position_.x + static_cast<int>(move_x * speed_), 
            position_.y + static_cast<int>(move_y * speed_)
        };
        if (environment_->abilityToMove(position_, newPos))
            return newPos;
    }
    Pair directEscape = {
        position_.x - static_cast<int>(nx * speed_),
        position_.y - static_cast<int>(ny * speed_)
    };
    if (environment_->abilityToMove(position_, directEscape))
        return directEscape;
    return position_;
}