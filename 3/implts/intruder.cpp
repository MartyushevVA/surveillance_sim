#include "intruder.h"

#include "mobile_platform.h"

Report Intruder::getSurrounding() const {
    std::vector<std::shared_ptr<Placeholder>> tokensInRange;
    for (const auto& token : environment_->getTokens())
        if ((environment_->howFar(position_, token->getPosition(), sensorRange_) <= 1) && environment_->hasLineOfSight(position_, token->getPosition()))
            tokensInRange.push_back(token);
    return {position_, tokensInRange};
}

MobilePlatform* Intruder::nearestPredatorWithinRange() const {
    Report report = getSurrounding();
    MobilePlatform* nearestPredator = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for (const auto& token : report.objects) {
        if (MobilePlatform* predator = dynamic_cast<MobilePlatform*>(token.get())) {
            if (minDistance = std::min(minDistance, environment_->howFar(position_, token->getPosition(), sensorRange_)) == environment_->howFar(position_, token->getPosition(), sensorRange_))
                nearestPredator = predator;
        }
    }
    return nearestPredator;
}