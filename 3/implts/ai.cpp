#include "ai.h"

#include <queue>
#include <set>

#include "module_types.h"
#include "mobile_platform.h"
#include "static_platform.h"
#include "suspect.h"
#include "environment.h"


void AI::updateNetworkForest() {
    allConnectedPlatforms_.clear();
    std::set<Platform*> visited;
    std::queue<Platform*> wip;
    for (auto platform : staticPlatforms_) {
        wip.push(platform);
        while (!wip.empty()) {
            auto current = wip.front();
            wip.pop();
            visited.insert(current);
            if (auto connection = current->findModuleOfType<ConnectionModule>())
                for (auto session : connection->getSessionList())
                    if (visited.find(session->getHost().get()) == visited.end())
                        wip.push(session->getHost().get());
        }
    }
    for (auto platform : visited)
        allConnectedPlatforms_.push_back(platform);
}

void AI::updateSpottedSuspects() {
    std::vector<Suspect*> wip;
    for (auto platform : allConnectedPlatforms_)
        if (auto sensor = platform->findModuleOfType<SensorModule>())
            for (auto placeholder : sensor->getSurrounding().objects)
                if (auto suspect = dynamic_cast<Suspect*>(placeholder.get()))
                    wip.push_back(suspect);
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
    for (auto platform : allConnectedPlatforms_) {
        if (auto sensor = platform->findModuleOfType<SensorModule>()) {
            Report report = sensor->getSurrounding();
            if (WeaponModule* weapon = platform->findModuleOfType<WeaponModule>()) {
                if (auto attackableSuspect = sensor->getVisibleSuspect(report)) {
                    weapon->attack(attackableSuspect->getPosition());
                }
            }
            if (MobilePlatform* officer = dynamic_cast<MobilePlatform*>(platform)) {
                Pair nextPos;
                if (auto attackableSuspect = sensor->getVisibleSuspect(report))
                    nextPos = officer->calculatePursuitMove(attackableSuspect->getPosition());
                else if (!spottedSuspects_.empty())
                    nextPos = officer->calculatePursuitMove(spottedSuspects_[0]->getPosition());
                else
                    nextPos = officer->calculateRandomMove();
                if (ConnectionModule* connection = officer->findModuleOfType<ConnectionModule>(); connection->isSafeForSystem(nextPos))
                    officer->move(nextPos);
            }
            /*if (auto connection = platform->findModuleOfType<ConnectionModule>()) {
                for (auto node : connection->getRouteList())
                    std::cout << "Host: (" << platform->getDescription() << ") | Gate: (" << node.gate->getHost()->getDescription()
                    << ") | Destination: (" << node.destination->getHost()->getDescription() << ")" << std::endl;
                std::cout << std::endl;
            }*/
        }
    }
}