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

std::unique_ptr<Module> Platform::extractModule(Module* module) {
    for (auto it = modules_.begin(); it != modules_.end(); ++it) {
        if (it->get() == module) {
            std::unique_ptr<Module> removedModule = std::move(*it);
            modules_.erase(it);
            return removedModule;
        }
    }
    return nullptr;
}