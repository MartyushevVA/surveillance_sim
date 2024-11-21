#include "mobile_platform.h"

Pair MobilePlatform::findPursuitableIntruder(Report report) const {
    for (const auto& token : report.objects)
        if (Intruder* intruder = dynamic_cast<Intruder*>(token.get()))
            if (environment_->hasLineOfSight(position_, intruder->getPosition()) && environment_->howFar(position_, intruder->getPosition(), speed_) <= 1)
                return intruder->getPosition();
    return {-1, 0};
}