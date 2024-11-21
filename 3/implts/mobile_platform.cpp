#include "mobile_platform.h"

Pair MobilePlatform::findPursuitableSuspect(Report report) const {
    for (const auto& token : report.objects)
        if (Suspect* suspect = dynamic_cast<Suspect*>(token.get()))
            if (environment_->hasLineOfSight(position_, suspect->getPosition()) && environment_->howFar(position_, suspect->getPosition(), speed_) <= 1)
                return suspect->getPosition();
    return {-1, 0};
}