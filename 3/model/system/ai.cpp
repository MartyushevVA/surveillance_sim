#include "ai.h"

#include <queue>
#include <set>

#include "environment.h"
#include "../modules/modules.h"
#include "../objects/objects.h"

void AI::updateNetworkForest() {
    allConnectedPlatforms_.clear();
    std::set<Platform*> visited;
    std::queue<Platform*> wip;
    for (auto platform : staticPlatforms_) {
        wip.push(platform.get());
        while (!wip.empty()) {
            auto current = wip.front();
            wip.pop();
            visited.insert(current);
            auto connection = current->findModuleOfType<ConnectionModule>();
            if (!connection) continue;
            for (auto session : connection->getSessionList()) {
                if (visited.find(session->getHost().get()) == visited.end()) {
                    wip.push(session->getHost().get());
                }
            }
        }
    }
    for (auto platform : visited)
        allConnectedPlatforms_.push_back(platform);
}

void AI::updateSpottedSuspects() {
    std::vector<Placeholder*> wip;
    for (auto platform : allConnectedPlatforms_)
        if (auto sensor = platform->findModuleOfType<SensorModule>())
            for (auto suspect : sensor->getSuspects())
                wip.push_back(suspect.get());
    for (auto it = spottedSuspects_.begin(); it != spottedSuspects_.end();) {
        if (std::find(wip.begin(), wip.end(), *it) == wip.end())
            it = spottedSuspects_.erase(it);
        else
            ++it;
    }
    for (auto suspect : wip)
        if (std::find(spottedSuspects_.begin(), spottedSuspects_.end(), suspect) == spottedSuspects_.end())
            spottedSuspects_.push_back(suspect);
}

void AI::eliminateAllSuspects() {
    updateNetworkForest();
    updateSpottedSuspects();
    for (auto platform : allConnectedPlatforms_)
        if (platform)
            platform->iterate(spottedSuspects_);
}