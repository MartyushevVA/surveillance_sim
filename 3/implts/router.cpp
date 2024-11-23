#include "module_types.h"

std::vector<ConnectionModule*> ConnectionModule::scanForModules(Pair position = {-1, 0}) {
    std::vector<ConnectionModule*> modulesInRange;
    if (position == Pair{-1, 0}) position = host_->getPosition();
    for (const auto& token : host_->getEnvironment()->getTokens())
        if (host_->getEnvironment()->howFar(position, token->getPosition(), range_) <= 1)
            if (auto module = host_->findModuleOfType<ConnectionModule>())
                modulesInRange.push_back(module);
    return modulesInRange;
}

void ConnectionModule::recursiveDiscord(ConnectionModule* gate, std::vector<ConnectionModule*>& targetList) {
    bool isEntered = false;
    for (auto node : targetList)
        if (auto it = std::find(routeList_.begin(), routeList_.end(), routeNode{gate, node}); it != routeList_.end()) {
            isEntered = true;
            routeList_.erase(it);
        }
    if (isEntered)
        for (auto module : sessionList_)
            module->recursiveDiscord(this, targetList);
}

bool ConnectionModule::establishConnection(ConnectionModule* target, bool isResponse = false) {
    if (sessionList_.size() < maxSessions_) {
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

bool ConnectionModule::closeConnection(ConnectionModule* target, bool isResponse = false) {
    if (auto node = std::find(sessionList_.begin(), sessionList_.end(), target); node != sessionList_.end()) {
        if (!isResponse)
            target->closeConnection(this, true);
        sessionList_.erase(node);
        for (auto it = routeList_.begin(); it != routeList_.end();)
            if (it->gate == target)
                it = routeList_.erase(it);
            else
                ++it;
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

void ConnectionModule::attachTo(Platform* host) const {
    if (host->getEnergyLevel() + energyConsumption_ <= host->getMaxEnergyLevel())
        host->installModule(std::make_unique<ConnectionModule>(*this));
}