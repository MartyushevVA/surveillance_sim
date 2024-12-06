#include "objects.h"

#include <algorithm>
#include <cmath>
#include <limits>

Platform* Suspect::nearestPredatorWithinRange() const {
    auto area = environment_->getArea(position_, sensorRange_);
    Platform* nearestPredator = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for (auto& [checkPos, token] : area)
        if (Platform* predator = dynamic_cast<Platform*>(token.get()))
            if (environment_->hasLineOfSight(position_, checkPos)) {
                double distance = environment_->howFar(position_, checkPos, sensorRange_);
                if (minDistance > distance) {
                    minDistance = distance;
                    nearestPredator = predator;
                }
            }
    return nearestPredator;
}