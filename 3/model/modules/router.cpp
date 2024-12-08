#include "modules.h"

#include <cmath>

#include "../objects/objects.h"

std::vector<ConnectionModule*> ConnectionModule::scanForModules(Pair pos) const {
    std::vector<ConnectionModule*> result;
    auto host = host_.lock();
    if (!host) return result;
    
    auto env = host->getEnvironment();
    if (!env) return result;
    
    auto area = env->getArea(pos, range_);
    for (const auto& [checkPos, token] : area)
        if (auto platform = std::dynamic_pointer_cast<Platform>(token))
            if (platform.get() != host.get())
                if (auto module = platform->findModuleOfType<ConnectionModule>())
                    result.push_back(module);
    return result;
}

bool ConnectionModule::establishConnection(ConnectionModule* target, bool isResponse) {
    if ((int)sessionList_.size() < maxSessions_ && std::find_if(routeList_.begin(), routeList_.end(),
    [target](const routeNode& a) {return a.destination == target;}) == routeList_.end()) {
        if (!isResponse)
            if (!target->establishConnection(this, true))
                return false;
        sessionList_.push_back(target);
        routeList_.push_back({target, target});
        std::vector<routeNode> partnersTargets = target->requestRouteList(this);
        for (auto& route : partnersTargets)
            routeList_.push_back({target, route.destination});
        for (auto session : sessionList_) 
            if (session != target) 
                session->recursiveRouteNodeImplementation(this, routeList_);
        return true;
    }
    return false;
}

bool ConnectionModule::closeConnection(ConnectionModule* target, bool isResponse) {
    if (!isResponse)
        target->closeConnection(this, true);
    sessionList_.erase(std::find(sessionList_.begin(), sessionList_.end(), target));

    routeList_.erase(std::remove_if(routeList_.begin(), routeList_.end(),
        [target](const routeNode& node) { return node.destination == target; }), routeList_.end());
    
    target->recursiveDiscord(this, routeList_);
    return true;
}

std::vector<routeNode> ConnectionModule::requestRouteList(ConnectionModule* source) const {
    std::vector<routeNode> routeList {};
    for (auto node : routeList_) {
        if (node.destination != source)
            routeList.push_back(node);
    }
    return routeList;
}

void ConnectionModule::recursiveRouteNodeImplementation(ConnectionModule* gate, std::vector<routeNode> routeList) {
    bool isEntered = false;
    for (auto& node : routeList)
        if (std::find_if(routeList_.begin(), routeList_.end(),
        [&node](const routeNode& a) {return a == node;}) == routeList_.end() && node.destination != this) {
            isEntered = true;
            routeList_.push_back(routeNode{gate, node.destination});
        }
    if (isEntered) {
        for (auto session : sessionList_)
            if (session != gate)
                session->recursiveRouteNodeImplementation(this, routeList);
    }
}

void ConnectionModule::recursiveDiscord(ConnectionModule* gate, std::vector<routeNode> targetList) {
    bool isEntered = false;
    for (auto node : targetList)
        if (auto it = std::find(routeList_.begin(), routeList_.end(), routeNode{gate, node.destination}); it != routeList_.end()) {
            isEntered = true;
            routeList_.erase(it);
        }
    if (isEntered)
        for (auto module : sessionList_)
            module->recursiveDiscord(this, targetList);
}

bool ConnectionModule::connectedToAI(const ConnectionModule* source) const {
    if (auto staticPlatform = dynamic_cast<StaticPlatform*>(host_.lock().get()); staticPlatform) {
        return true;
    }
    for (auto node : routeList_)
        if (auto staticPlatform = dynamic_cast<StaticPlatform*>(node.destination->getHost().get()); staticPlatform && node.gate != source) {
            return true;
        }
    return false;
}

bool ConnectionModule::isSafeForSystem(Pair newPosition) const {
    auto host = host_.lock();
    if (!host) return false;    
    auto staticPlatform = getConnectedToAIDirectly();
    if (!staticPlatform) return false;    
    for (const auto& session : sessionList_) {
        if (!session) continue;
        auto connectedPlatform = session->getHost();
        if (!connectedPlatform) continue;
        auto distance = host->getEnvironment()->calculateDistance(newPosition, connectedPlatform->getPosition());
        if (distance > range_)
            return false;
    }
    return true;
}

StaticPlatform* ConnectionModule::getConnectedToAIDirectly() const {
    if (auto staticPlatform = dynamic_cast<StaticPlatform*>(host_.lock().get()); staticPlatform)
        return staticPlatform;
    for (auto node : routeList_)
        if (auto staticPlatform = dynamic_cast<StaticPlatform*>(node.destination->getHost().get()); staticPlatform)
            return staticPlatform;
    return nullptr;
}

void ConnectionModule::update() {
    auto host = host_.lock();
    if (!host) return;
    
    auto newNeighbors = scanForModules(host->getPosition());
    for (auto* neighbor : newNeighbors) {
        if (!neighbor) continue;
        auto neighborHost = neighbor->getHost();
        if (!neighborHost) continue;
        if (std::find(sessionList_.begin(), sessionList_.end(), neighbor) == sessionList_.end())
            establishConnection(neighbor);
    }
    
    auto it = sessionList_.begin();
    while (it != sessionList_.end()) {
        auto* session = *it;
        if (!session || std::find(newNeighbors.begin(), newNeighbors.end(), session) == newNeighbors.end())
            it = sessionList_.erase(it);
        else
            ++it;
    }
}

void ConnectionModule::turnOn() {
    setIsOn(true);
    host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() + energyConsumption_);
}

void ConnectionModule::turnOff() {
    setIsOn(false);
    host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() - energyConsumption_);
}