#include "base_platform.h"

#include "../modules/base_module.h"

void Platform::installModule(std::shared_ptr<Module> module) {
    if (!module->attachableTo(shared_from_this()))
        throw std::runtime_error("Module is not attachable to the platform");
    module->setHost(shared_from_this());
    modules_.push_back(module);
}