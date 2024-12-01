#include "objects.h"

#include <algorithm>
#include <cmath>
#include <limits>

Platform* Suspect::nearestPredatorWithinRange() const {
    Platform* nearestPredator = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for (int dx = -sensorRange_; dx <= sensorRange_; dx++)
        for (int dy = -sqrt(sensorRange_ * sensorRange_ - dx * dx); dy <= sqrt(sensorRange_ * sensorRange_ - dx * dx); dy++) {
            Pair checkPos{position_.x + dx, position_.y + dy};
            if (checkPos.x < 0 || checkPos.y < 0 || 
                checkPos.x >= environment_->getSize().x || 
                checkPos.y >= environment_->getSize().y)
                continue;
            if (Platform* predator = dynamic_cast<Platform*>(environment_->getToken(checkPos).get()))
                if (environment_->hasLineOfSight(position_, checkPos)) {
                    double distance = environment_->howFar(position_, checkPos, sensorRange_);
                    if (minDistance > distance) {
                        minDistance = distance;
                        nearestPredator = predator;
                    }
                }
    }
    return nearestPredator;
}