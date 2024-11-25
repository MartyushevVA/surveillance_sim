#include "module_types.h"

#include "platform.h"
#include "environment.h"

std::vector<ConnectionModule*> ConnectionModule::scanForModules(Pair position) {
    std::vector<ConnectionModule*> modulesInRange;
    if (position == Pair{-1, 0}) position = host_.lock()->getPosition();
    auto hostPtr = host_.lock();
    for (const auto& token : hostPtr->getEnvironment()->getTokens())
        if (hostPtr->getEnvironment()->howFar(position, token->getPosition(), range_) <= 1
        && hostPtr->getPosition() != token->getPosition()) {
            //std::cout << "connectionModule6: " << token->getPosition().x << ' ' << token->getPosition().y << std::endl;
            if (auto platform = dynamic_cast<Platform*>(token.get()))
                if (auto module = platform->findModuleOfType<ConnectionModule>())
                    modulesInRange.push_back(module);
    }
    return modulesInRange;
}



bool ConnectionModule::establishConnection(ConnectionModule* target, bool isResponse) {
    if (sessionList_.size() < maxSessions_ && std::find_if(routeList_.begin(), routeList_.end(),
    [target](const routeNode& a) {return a.destination == target;}) == routeList_.end()) {
        if (!isResponse)
            if (target->establishConnection(this, true))
                sessionList_.push_back(target);
        else
            sessionList_.push_back(target);
        routeList_.push_back({target, target});
        target->recursiveRouteNodeImplementation(this, routeList_);
        applyRouteList(target->requestRouteList(this));
        return true;
    }
    return false;
}

bool ConnectionModule::closeConnection(ConnectionModule* target, bool isResponse) {
    if (!isResponse)
        target->closeConnection(this, true);
    sessionList_.erase(std::find(sessionList_.begin(), sessionList_.end(), target));
    target->recursiveDiscord(this, routeList_);
    return true;
}

void ConnectionModule::applyRouteList(std::vector<routeNode> routeList) {
    for (auto& node : routeList)
        if (std::find_if(routeList_.begin(), routeList_.end(),
        [node](const routeNode& a) {return a.destination == node.destination;}) == routeList_.end())
            routeList_.push_back(node);
}

std::vector<routeNode> ConnectionModule::requestRouteList(ConnectionModule* source) const {
    std::vector<routeNode> routeList;
    for (auto node : sessionList_)
        if (node != source)
            routeList.insert(routeList.end(), node->getRouteList().begin(), node->getRouteList().end());
    return routeList;
}

void ConnectionModule::recursiveRouteNodeImplementation(ConnectionModule* gate, std::vector<routeNode> routeList) {
    bool isEntered = false;
    for (auto& node : routeList)
        if (std::find_if(routeList_.begin(), routeList_.end(),
        [node](const routeNode& a) {return a.destination == node.destination;}) == routeList_.end()) {
            isEntered = true;
            routeList_.push_back(routeNode{gate, node.destination});
        }
    if (isEntered)
        for (auto module : sessionList_)
            if (module != gate)
                module->recursiveRouteNodeImplementation(this, routeList);
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

bool ConnectionModule::attachableTo(std::shared_ptr<Platform> host) const {
    if (!host)
        throw std::invalid_argument("Host is not set");
    return (host->getEnergyLevel() + energyConsumption_ <= host->getMaxEnergyLevel())
    && (host->getModules().size() + slotsOccupied_ <= host->getSlotCount());
}

void ConnectionModule::refresh() {}

void ConnectionModule::positionRelatedUpdate(Pair newPosition) {
    std::vector<ConnectionModule*> newNeighborsList = scanForModules(newPosition);
    for (auto module : newNeighborsList)
        if (std::find(sessionList_.begin(), sessionList_.end(), module) == sessionList_.end()) { //если это новый сосед
            establishConnection(module, false);
        }
    for (auto module : sessionList_)
        if (std::find(newNeighborsList.begin(), newNeighborsList.end(), module) == newNeighborsList.end()) {//если это больше не сосед
            closeConnection(module, false);
        }
}

void ConnectionModule::setUp() {
    std::vector<ConnectionModule*> neighborsList = scanForModules(host_.lock()->getPosition());
    for (auto module : neighborsList)
        establishConnection(module, false);
}