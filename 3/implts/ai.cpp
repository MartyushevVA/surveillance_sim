#include "ai.h"

#include "module_types.h"
#include "mobile_platform.h"
#include "suspect.h"
#include "environment.h"
#include <queue>

void AI::updateConnections() {
    allConnectedPlatforms_.clear();
    std::set<Platform*> visited;
    std::queue<Platform*> wip;
    for (auto platform : staticPlatforms_) {
        wip.push(platform);
        while (!wip.empty()) {
            auto current = wip.front();
            wip.pop();
            visited.insert(current);
            if (auto connection = current->findModuleOfType<ConnectionModule>()) {
                for (auto session : connection->getSessionList()) {
                    if (visited.find(session->getHost().get()) == visited.end())
                        wip.push(session->getHost().get());
                }
            }
        }
    }
    allConnectedPlatforms_.insert(allConnectedPlatforms_.end(), visited.begin(), visited.end());
}

void AI::updateSuspects() {
    std::vector<Suspect*> wip;
    for (auto platform : allConnectedPlatforms_) {
        if (auto officer = dynamic_cast<MobilePlatform*>(platform))
            if (auto sensor = officer->findModuleOfType<SensorModule>())
                for (auto placeholder : sensor->getSurrounding().objects)
                    if (auto suspect = dynamic_cast<Suspect*>(placeholder.get()))
                        wip.push_back(suspect);
    for (auto it = suspects_.begin(); it != suspects_.end();) {
        if (std::find(wip.begin(), wip.end(), *it) == wip.end())
            it = suspects_.erase(it);
        else
            ++it;
    }
    for (auto suspect : wip)
        if (std::find(suspects_.begin(), suspects_.end(), suspect) == suspects_.end())
            suspects_.push_back(suspect);
}
