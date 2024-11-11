#include "platform.h"
#include "module.h"
#include <stdexcept>

void Platform::installModule(std::unique_ptr<Module> module) {
    if (modules_.size() + module->getSlotsOccupied() <= slotCount_) {
        module->setHost(this);
        modules_.push_back(std::move(module));
    }
    else {
        throw std::runtime_error("Not enough slots available on the platform");
    }
}

/*std::unique_ptr<Module> Platform::removeModule(std::unique_ptr<Module> module) {
    
}*/