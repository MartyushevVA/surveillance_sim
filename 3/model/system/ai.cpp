#include "ai.h"

#include <queue>
#include <set>
#include <thread>
#include <future>
#include "thread_pool.h"

#include <iostream>

#include "../modules/modules.h"

AI::AI(std::shared_ptr<Environment> environment) : environment_(environment) {
    auto environmentPtr = environment_.lock();
    for (const auto& [position, token] : environmentPtr->getTokens())
        if (auto provider = std::dynamic_pointer_cast<StaticPlatform>(token)) {
            auto host = provider->findModuleOfType<ConnectionModule>();
            if (!host) continue;
            mainHosts_.push_back(host);
            allConnectedPlatforms_.push_back(host);
        }
}

void AI::syncHosts() {
    for (auto host : mainHosts_) {
        auto staticPlatform = std::dynamic_pointer_cast<StaticPlatform>(host->getHost());
        if (!staticPlatform) continue;
        staticPlatform->setAI(shared_from_this());
    }
}

void AI::addConnectedPlatform(std::shared_ptr<ConnectionModule> platform) {
    if (std::find_if(allConnectedPlatforms_.begin(), allConnectedPlatforms_.end(), [platform](std::shared_ptr<ConnectionModule> host) {
        return host->getHost() == platform->getHost();}) == allConnectedPlatforms_.end())
        allConnectedPlatforms_.push_back(platform);
}

void AI::eliminateAllSuspects() {
    const size_t threadCount = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;
    ThreadPool pool(threadCount);
    for (auto router : allConnectedPlatforms_) {
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