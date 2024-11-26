#include <random>

#include "placeholder.h"
#include "mobile_platform.h"
#include "suspect.h"

#include "environment.h"

void Placeholder::move(Pair position) {
    if (abilityToMove(position)) {
        positionRelatedUpdate(position);
        setPosition(position);
    }
}

bool Placeholder::abilityToMove(Pair position) const {
    if (!environment_) return false;
    Pair size = environment_->getSize();
    if (position.x < 0 || position.x >= size.x || 
        position.y < 0 || position.y >= size.y)
        return false;
    return environment_->getCellType(position) == CellType::Empty || position == position_;
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
        if (abilityToMove(newPos))
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
    double adjusted_dx = nx * speed * cos(angle_adjustment);
    double adjusted_dy = ny * speed * sin(angle_adjustment);

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
        if (abilityToMove(newPos))
            return newPos;
    }
    Pair directEscape = {
        position_.x - static_cast<int>(nx * speed_),
        position_.y - static_cast<int>(ny * speed_)
    };
    if (abilityToMove(directEscape))
        return directEscape;
    return position_;
}