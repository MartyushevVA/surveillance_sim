#include "base_platform.h"

#include "../modules/base_module.h"

void Platform::installModule(std::shared_ptr<Module> module) {
    if (modules_.size() + module->getSlotsOccupied() <= slotCount_) {
        module->setHost(shared_from_this());
        modules_.push_back(module);
        if (energyLevel_ + module->getEnergyConsumption() <= maxEnergyLevel_)
            module->turnOn();
    }
    else
        throw std::runtime_error("Module is not attachable to the platform");
}