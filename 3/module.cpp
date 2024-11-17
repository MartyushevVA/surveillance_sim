#include <stdexcept>
#include "sensor_module.h"
#include "module.h"
#include "platform.h"

bool Module::isInRange(Pair target) const {
    return (target.x - host_->getPosition().x) * (target.x - host_->getPosition().x) +
           (target.y - host_->getPosition().y) * (target.y - host_->getPosition().y) <= range_ * range_;
}