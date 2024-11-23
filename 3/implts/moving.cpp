#include "mobile_platform.h"
#include "suspect.h"
#include "placeholder.h"

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
    srand(time(nullptr));
    int dx = ((rand() % (2*speed_ + 1)) - speed_);
    int dy = rand() % 2 ? sqrt(speed_ * speed_ - dx * dx) : -sqrt(speed_ * speed_ - dx * dx);
    return {position_.x + dx, position_.y + dy};
}

Pair MobilePlatform::calculatePursuitMove(Pair target) const {
    int dx = target.x - position_.x;
    int dy = target.y - position_.y;
    double distance = sqrt(dx * dx + dy * dy);
    double bo_sinus = dy / distance;
    double bo_cosinus = dx / distance;
    return {position_.x + (int)(bo_cosinus * speed_), position_.y + (int)(bo_sinus * speed_)};
}

Pair Suspect::calculateAvoidanceMove(Pair threat) const {
    double dx = threat.x - position_.x;
    double dy = threat.y - position_.y;
    double distance = sqrt(dx * dx + dy * dy);
    double nx = dx / distance;
    double ny = dy / distance;
    double perpx = -ny;
    double perpy = nx;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angle_dist(1.5707963267948966, 4.71238898038469);
    double angle = angle_dist(gen);

    double move_x = cos(angle);
    double move_y = sin(angle);
    double rotated_x = move_x * (-nx) - move_y * (-ny);
    double rotated_y = move_x * (-ny) + move_y * nx;
    return {position_.x + (int)rotated_x * speed_, position_.y + (int)rotated_y * speed_};
}