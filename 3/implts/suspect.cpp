#include "suspect.h"

#include "platform.h"
#include "environment.h"

Report Suspect::getSurrounding() const {
    std::vector<std::shared_ptr<Placeholder>> tokensInRange;
    for (const auto& token : environment_->getTokens())
        if ((environment_->howFar(position_, token->getPosition(), sensorRange_) <= 1) && environment_->hasLineOfSight(position_, token->getPosition()))
            tokensInRange.push_back(token);
    return {position_, tokensInRange};
}

Platform* Suspect::nearestPredatorWithinRange() const {
    Report report = getSurrounding();
    Platform* nearestPredator = nullptr;
    double minDistance = std::numeric_limits<double>::max();
    for (const auto& token : report.objects) {
        if (Platform* predator = dynamic_cast<Platform*>(token.get())) {
            if (minDistance = std::min(minDistance, environment_->howFar(position_, token->getPosition(), sensorRange_)) == environment_->howFar(position_, token->getPosition(), sensorRange_))
                nearestPredator = predator;
        }
    }
    return nearestPredator;
}