#include <stdexcept>
#include "sensor_module.h"
#include "module.h"
#include "platform.h"

/*bool Module::isInRange(Pair target) const {
    if (!host_) return false;
    if (SensorModule* sensor = findModuleOfType<SensorModule>()) {

}*/

void Module::attachTo(Platform* host) const {
    if (!host) return;
    try {
        host->installModule(std::make_unique<Module>(*this));
    }
    catch (const std::runtime_error& e) {
        throw std::runtime_error("Failed to install module: " + std::string(e.what()));
    }
}
