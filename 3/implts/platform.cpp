#include "platform.h"

#include "module.h"

void Platform::installModule(std::shared_ptr<Module> module) {
    if (!module->attachableTo(shared_from_this()))
        throw std::runtime_error("Module is not attachable to the platform");
    module->setHost(shared_from_this());
    modules_.push_back(std::move(module));
}

void Platform::update() {
    for (auto& module : modules_)
        module->update();
}