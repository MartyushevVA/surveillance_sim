#include "module.h"

#include "platform.h"

bool Module::isInRange(Pair target) const {
    auto hostPtr = host_.lock();
    if (hostPtr)
        return (target.x - hostPtr->getPosition().x) * (target.x - hostPtr->getPosition().x) +
           (target.y - hostPtr->getPosition().y) * (target.y - hostPtr->getPosition().y) <= range_ * range_;
    return false;
}