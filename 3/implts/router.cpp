#include <queue>

#include "module_types.h"

#include "platform.h"
#include "environment.h"

std::vector<ConnectionModule*> ConnectionModule::scanForModules(Pair position) {
    std::vector<ConnectionModule*> modulesInRange;
    if (position == Pair{-1, 0}) position = host_.lock()->getPosition();
    auto hostPtr = host_.lock();
    auto env = hostPtr->getEnvironment();
    for (int dx = -range_; dx <= range_; dx++)
        for (int dy = -sqrt(range_ * range_ - dx * dx); dy <= sqrt(range_ * range_ - dx * dx); dy++) {
            Pair checkPos{position.x + dx, position.y + dy};
            if (checkPos == hostPtr->getPosition() || 
                checkPos.x < 0 || checkPos.y < 0 || 
                checkPos.x >= env->getSize().x || 
                checkPos.y >= env->getSize().y)
                continue;
            if (auto token = env->getToken(checkPos))
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
    //std::cout << host_.lock()->getDescription() << " is implementing route list" << std::endl;
    //std::cout << gate->getHost()->getDescription() << " is the gate" << std::endl;
    bool isEntered = false;
    for (auto& node : routeList)
        if (std::find_if(routeList_.begin(), routeList_.end(),
        [&node](const routeNode& a) {return a.destination == node.destination;}) == routeList_.end() && node.destination != this) {
            isEntered = true;
            routeList_.push_back(routeNode{gate, node.destination});
            //std::cout << node.destination->getHost()->getDescription() << " is being added to route list" << std::endl;
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

bool ConnectionModule::attachableTo(std::shared_ptr<Platform> host) const {
    if (!host)
        throw std::invalid_argument("Host is not set");
    return (host->getEnergyLevel() + energyConsumption_ <= host->getMaxEnergyLevel())
    && (host->getModules().size() + slotsOccupied_ <= host->getSlotCount());
}

void ConnectionModule::update() {
    std::vector<ConnectionModule*> newNeighborsList = scanForModules();
    for (auto node : routeList_)
            std::cout << "Host: (" << host_.lock()->getDescription() << ") | Gate: (" << node.gate->getHost()->getDescription()
            << ") | Destination: (" << node.destination->getHost()->getDescription() << ")" << std::endl;
    std::cout << std::endl;
    
    for (auto module : newNeighborsList)
        if (std::find(sessionList_.begin(), sessionList_.end(), module) == sessionList_.end()) {
            establishConnection(module, false);
        }
    for (auto module : sessionList_)
        if (std::find(newNeighborsList.begin(), newNeighborsList.end(), module) == newNeighborsList.end()) {
            closeConnection(module, false);
        }
}

void ConnectionModule::setUp() {
    if (host_.lock())
        host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() + energyConsumption_);
    std::vector<ConnectionModule*> neighborsList = scanForModules(host_.lock()->getPosition());
    for (auto module : neighborsList)
        establishConnection(module, false);
}