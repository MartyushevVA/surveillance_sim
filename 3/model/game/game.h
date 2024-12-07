#pragma once

#include <atomic>
#include <mutex>
#include <thread>

#include "../system/ai.h"
#include "../system/environment.h"
#include "../../import/import.h"
#include "../../view/graphics.h"

class Game {
private:
    std::vector<std::thread> gameThreads_;

    AI ai_;
    Environment environment_;
    Graphics graphics_;
    std::chrono::milliseconds updateInterval_;
    std::mutex environmentMutex_;
    std::atomic<bool> isRunning_{false};
    
    void initializeField(const GameConfig& config);
    void suspectThread(std::shared_ptr<Suspect> suspect);
    void platformThread(std::shared_ptr<MobilePlatform> platform);
    void updateEntitiesParallel();
    void cleanupThreads();
public:
    Game(const std::string& gameConfigFile, const std::string& graphicsConfigFile) :
        ai_(&environment_),
        graphics_(graphicsConfigFile) {
        initializeField(Import::importGameConfig(gameConfigFile));
    }    
    void start();
    ~Game() {cleanupThreads();}
};