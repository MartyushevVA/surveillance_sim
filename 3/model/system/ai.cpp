#include "ai.h"

#include <queue>
#include <set>
#include <thread>
#include <future>
#include <vector>
#include "thread_pool.h"

#include "environment.h"
#include "../modules/modules.h"
#include "../objects/objects.h"

void AI::getNetworkForest() {
    allConnectedPlatforms_.clear();

    std::set<Platform*> visited;
    std::queue<Platform*> wip;
    for (const auto& staticPlatform : staticPlatforms_) {
        if (!staticPlatform) continue;
        wip.push(staticPlatform.get());
        visited.insert(staticPlatform.get());
        allConnectedPlatforms_.push_back(staticPlatform.get());
        
        while (!wip.empty()) {
            auto current = wip.front();
            wip.pop();
            auto connection = current->findModuleOfType<ConnectionModule>();
            if (!connection) continue;
            
            auto sessions = connection->getSessionList();
            
            for (const auto& connectedModule : sessions) {
                if (!connectedModule) continue;
                
                auto connectedPlatform = connectedModule->getHost();
                if (!connectedPlatform) continue;
                if (visited.insert(connectedPlatform.get()).second) {
                    allConnectedPlatforms_.push_back(connectedPlatform.get());
                    wip.push(connectedPlatform.get());
                }
            }
        }
    }
}

void AI::eliminateAllSuspects() {
    const size_t threadCount = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;
    ThreadPool pool(threadCount);
    std::mutex environmentMutex;
    
    getNetworkForest();
    auto platforms = allConnectedPlatforms_;

    for (auto platform : platforms) {
        if (!platform) continue;
        
        futures.emplace_back(pool.enqueue([platform, &environmentMutex]() {
            std::lock_guard<std::mutex> lock(environmentMutex);
            platform->iterate();
        }));
    }

    for (auto& future : futures)
        future.get();
}

void AI::addSuspects(std::map<Pair, std::shared_ptr<Placeholder>> suspects) {
    for (auto [pos, suspect] : suspects) {
        for (auto it = spottedSuspects_.begin(); it != spottedSuspects_.end();)
            if (it->second == suspect)
                it = spottedSuspects_.erase(it);
            else
                ++it;
        spottedSuspects_[pos] = suspect;
    }
}

void AI::removeSuspect(std::shared_ptr<Placeholder> suspect) {
    if (!suspect)
        return;
    
    std::mutex suspectsMutex;

    std::lock_guard<std::mutex> lock(suspectsMutex);
    for (auto it = spottedSuspects_.begin(); it != spottedSuspects_.end();)
        if (it->second == suspect)
            it = spottedSuspects_.erase(it);
        else
            ++it;
}