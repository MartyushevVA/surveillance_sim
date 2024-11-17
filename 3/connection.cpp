#include "connection_module.h"
#include "platform.h"

std::vector<ConnectionModule*> ConnectionModule::scanForModules() {
    std::vector<ConnectionModule*> modulesInRange;
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
    std::vector<ConnectionModule*> newModules = scanForModules();
    if (std::find(newModules.begin(), newModules.end(), target) != newModules.end()
    && sessionList_.size() < maxSessions_) {
        if (!isResponse)
            if (target->establishConnection(this, true))
                sessionList_.push_back(target);
        else
            sessionList_.push_back(target);
        routeList_.push_back({target, target});
        return true;
    }
    return false;
}

bool ConnectionModule::closeConnection(ConnectionModule* target) {
    if (auto node = std::find(sessionList_.begin(), sessionList_.end(), target); node != sessionList_.end()) {
        target->closeConnection(this);
        sessionList_.erase(node);
        return true;
    }
    return false;
}

std::vector<routeNode> ConnectionModule::requestRouteList() const {
    auto isDuplicate = [](const routeNode& a, const routeNode& b) {
        return a.destination == b.destination;
    };
    auto customCompare = [isDuplicate](const routeNode& a, const routeNode& b) {
        return a.destination < b.destination;
    };
    std::vector<routeNode> routeList;
    for (auto node : sessionList_){
        routeList.insert(routeList.end(), node->getRouteList().begin(), node->getRouteList().end());
    }
    std::sort(routeList.begin(), routeList.end(), customCompare);
    auto last = std::unique(routeList.begin(), routeList.end(), isDuplicate);
    routeList.erase(last, routeList.end());
    return routeList;
}

void ConnectionModule::applyRouteList(std::vector<routeNode> routeList) {
    auto isDuplicate = [](const routeNode& a, const routeNode& b) {
        return a.destination == b.destination;
    };
    auto customCompare = [isDuplicate](const routeNode& a, const routeNode& b) {
        return a.destination < b.destination;
    };
    std::vector<routeNode> newRouteList;
    newRouteList.insert(newRouteList.end(), routeList.begin(), routeList.end());
    newRouteList.insert(newRouteList.end(), routeList_.begin(), routeList_.end());
    std::sort(newRouteList.begin(), newRouteList.end(), customCompare);
    auto last = std::unique(newRouteList.begin(), newRouteList.end(), isDuplicate);
    newRouteList.erase(last, newRouteList.end());
    routeList_ = newRouteList;
}
