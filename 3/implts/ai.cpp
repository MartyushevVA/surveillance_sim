#include "ai.h"

#include "platform.h"
#include "module_types.h"
#include <queue>

void AI::updateConnections() {
    temporarilyConnectedPlatforms_.clear();
    std::set<Platform*> visited;
    std::queue<Platform*> wip;
    for (auto platform : staticPlatforms_) {
        wip.push(platform);
        while (!wip.empty()) {
            auto current = wip.front();
            wip.pop();
            visited.insert(current);
            if (auto connection = current->findModuleOfType<ConnectionModule>()) {
                for (auto session : connection->getSessionList())
                    if (visited.find(session->getHost().get()) == visited.end())
                        wip.push(session->getHost().get());
            }
        }
    }
    temporarilyConnectedPlatforms_.insert(temporarilyConnectedPlatforms_.end(), visited.begin(), visited.end());
}
