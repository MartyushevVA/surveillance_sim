#include "ai.h"

#include <queue>
#include <set>
#include <thread>
#include <future>
#include "thread_pool.h"

#include "../modules/modules.h"

AI::AI(std::shared_ptr<Environment> environment) : environment_(environment) {
    auto environmentPtr = environment_.lock();
    for (const auto& [position, token] : environmentPtr->getTokens())
        if (auto provider = std::dynamic_pointer_cast<StaticPlatform>(token)) {
            auto host = provider->findModuleOfType<ConnectionModule>();
            if (!host) continue;
            addMainHost(host);
        }
}

void AI::addMainHost(std::shared_ptr<ConnectionModule> host) {
    if (!host)
        return;
    mainHosts_.push_back(host);
}

void AI::getNetworkForest() {
    allConnectedPlatforms_.clear();

    std::set<std::shared_ptr<ConnectionModule>> visited;
    std::queue<std::shared_ptr<ConnectionModule>> wip;
    for (const auto& host : mainHosts_) {
        if (!host) continue;
        wip.push(host);
        visited.insert(host);
        allConnectedPlatforms_.push_back(host);
        
        while (!wip.empty()) {
            auto current = wip.front();
            wip.pop();
            auto sessions = current->getSessionList();
            
            for (const auto& session : sessions) {
                if (visited.insert(session.lock()).second) {
                    allConnectedPlatforms_.push_back(session.lock());
                    wip.push(session.lock());
                }
            }
        }
    }
}

void AI::eliminateAllSuspects() {
    const size_t threadCount = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;
    ThreadPool pool(threadCount);
    getNetworkForest();
    auto routers = allConnectedPlatforms_;

    for (auto router : routers) {
        if (!router) continue;
        auto platform = router->getHost();
        futures.emplace_back(pool.enqueue([platform]() {
            platform->iterate();
        }));
    }

    for (auto& future : futures)
        future.get();
}

void AI::addSuspects(std::map<Pair, std::shared_ptr<Suspect>> suspects) {
    for (auto [pos, suspect] : suspects) {
        for (auto it = spottedSuspects_.begin(); it != spottedSuspects_.end();)
            if (it->second == suspect)
                it = spottedSuspects_.erase(it);
            else
                ++it;
        spottedSuspects_[pos] = suspect;
    }
}

void AI::removeSuspect(std::shared_ptr<Suspect> suspect) {
    if (!suspect)
        return;
    for (auto it = spottedSuspects_.begin(); it != spottedSuspects_.end();)
        if (it->second == suspect)
            it = spottedSuspects_.erase(it);
        else
            ++it;
}