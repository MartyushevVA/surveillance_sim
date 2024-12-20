#include "ai.h"

#include <queue>
#include <set>
#include <thread>
#include <future>

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
    std::unique_lock<std::shared_mutex> lock(platformsMutex_);
    if (std::find(allConnectedPlatforms_.begin(), allConnectedPlatforms_.end(), platform) 
        == allConnectedPlatforms_.end()) {
        allConnectedPlatforms_.push_back(platform);
    }
}

#include <iostream>

void AI::eliminateAllSuspects() {
    std::vector<std::future<void>> futures;
    
    {
        std::shared_lock<std::shared_mutex> lock(platformsMutex_);
        futures.reserve(allConnectedPlatforms_.size());
        for (auto& router : allConnectedPlatforms_) {
            if (!router) continue;
            auto platform = router->getHost();
            if (!platform) continue;
            futures.emplace_back(std::async(std::launch::async, [platform]() {
                //std::cout << "Iterating platform: " << platform->getDescription() << std::endl;
                platform->iterate();
            }));
        }
    }

    for (auto& future : futures) {
        future.get();
    }
}

void AI::addSuspects(std::map<Pair, std::shared_ptr<Suspect>> suspects) {
    std::unique_lock<std::shared_mutex> lock(suspectsMutex_);
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
    std::unique_lock<std::shared_mutex> lock(platformsMutex_);
    for (auto it = spottedSuspects_.begin(); it != spottedSuspects_.end();)
        if (it->second == suspect)
            it = spottedSuspects_.erase(it);
        else
            ++it;
}