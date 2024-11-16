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

void ConnectionModule::implementRouteNodes(std::vector<routeNode> routeNodes) {

}

void ConnectionModule::establishConnection(Module& module) {

}

void ConnectionModule::closeConnection(Module& module) {

}

bool ConnectionModule::isConnectedToAI() const {

}