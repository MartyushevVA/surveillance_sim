#include "ai.h"

#include "module_types.h"
#include "mobile_platform.h"
#include "static_platform.h"
#include "suspect.h"
#include "environment.h"
#include <queue>

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
    allConnectedPlatforms_.insert(allConnectedPlatforms_.end(), visited.begin(), visited.end());
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
                auto attackableSuspect = platform->getEnvironment()->getSuspect(report.objects);
                if (attackableSuspect)
                    if (weapon->attack(attackableSuspect->getPosition()))
                        continue;
            }
            if (MobilePlatform* officer = dynamic_cast<MobilePlatform*>(platform)) {
                if (auto attackableSuspect = platform->getEnvironment()->getSuspect(report.objects))
                    officer->move(officer->calculatePursuitMove(attackableSuspect->getPosition()));
                else
                    officer->move(officer->calculatePursuitMove(spottedSuspects_.front()->getPosition()));
            }
        }
    }
}