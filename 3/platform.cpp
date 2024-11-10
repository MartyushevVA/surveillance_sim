#include "platform.h"
#include "module.h"

void Platform::installModule(std::unique_ptr<Module> module) {
    if (modules_.size() + module->getSlotsOccupied() <= slotCount_) {
        modules_.push_back(std::move(module));
    }
}

/*std::unique_ptr<Module> Platform::removeModule(std::unique_ptr<Module> module) {
    
}*/