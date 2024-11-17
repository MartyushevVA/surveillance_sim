#include "connection_module.h"
#include "platform.h"

std::vector<Module*> ConnectionModule::scanForModules() {
    std::vector<Module*> modulesInRange;
    for (const auto& token : host_->getEnvironment()->getTokens()) {
        int x = token->getPosition().x;
        int y = token->getPosition().y;
        if ((x - host_->getPosition().x) * (x - host_->getPosition().x) + (y - host_->getPosition().y) * (y - host_->getPosition().y) <= getRange() * getRange())
            if (auto module = host_->findModuleOfType<ConnectionModule>())
                modulesInRange.push_back(module);
    }
    return modulesInRange;
}

bool ConnectionModule::establishConnection(ConnectionModule* target, bool isResponse = false) {
    std::vector<Module*> newModules = scanForModules();
    if (std::find(newModules.begin(), newModules.end(), target) != newModules.end()
    && sessionList_.size() < maxSessions_) {
        if (!isResponse)
            if (target->establishConnection(this, true))
                sessionList_.push_back(target);
        else
            sessionList_.push_back(target);
        return true;
    }
    return false;
}