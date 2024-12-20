#include <iostream>
#include <chrono>
#include <vector>
#include <functional>
#include <memory>
#include <cmath>
#include <future>
#include <random>
#include "system/environment.h"
#include "objects/objects.h"
#include "modules/modules.h"

// Function to generate random modules
std::shared_ptr<Module> generateRandomModule() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> moduleTypeDist(0, 2); // 0: Connection, 1: Sensor, 2: Weapon
    std::uniform_int_distribution<int> energyDist(4, 10);
    std::uniform_int_distribution<int> rangeDist(10, 20);
    std::uniform_int_distribution<int> slotsOccupiedDist(1, 1); // Assuming 1 slot for simplicity

    int moduleType = moduleTypeDist(generator);
    if (moduleType == 0) {
        return std::make_shared<ConnectionModule>(
            slotsOccupiedDist(generator),
            energyDist(generator),
            rangeDist(generator),
            3 // maxSessions
        );
    } else if (moduleType == 1) {
        return std::make_shared<SensorModule>(
            slotsOccupiedDist(generator),
            energyDist(generator),
            rangeDist(generator),
            SensorType::Optical
        );
    } else {
        return std::make_shared<WeaponModule>(
            slotsOccupiedDist(generator),
            energyDist(generator),
            rangeDist(generator),
            std::chrono::milliseconds(500)
        );
    }
}

// Function to generate random platforms
std::shared_ptr<Environment> generateRandomPlatforms(int numPlatforms) {
    auto env = std::make_shared<Environment>(Pair{100, 100});
    std::default_random_engine generator;
    std::uniform_int_distribution<int> positionDist1(0, 99); // Random positions within a range
    std::uniform_int_distribution<int> positionDist2(0, 99); // Random positions within a range
    std::uniform_int_distribution<int> slotCountDist(1, 4); // Random slot count

    for (int i = 0; i < numPlatforms; ++i) {
        int x = positionDist1(generator);
        int y = positionDist2(generator);
        int slotCount = slotCountDist(generator);
        std::vector<std::shared_ptr<Module>> modules;

        // Generate random modules for the platform
        for (int j = 0; j < slotCount; ++j) {
            modules.push_back(generateRandomModule());
        }

        // Create platform based on random type
        std::shared_ptr<MobilePlatform> platform;
        platform = std::make_shared<MobilePlatform>(
            Pair{x, y}, 
            env, // Assuming weak_from_this() is handled in the environment
            "Random Mobile Platform", 
            100, // maxEnergyLevel
            slotCount, 
            3 // speed
        );

        // Install generated modules into the platform
        for (const auto& module : modules) {
            platform->installModule(module);
        }

        env->addToken(platform);
    }
    auto host = std::make_shared<StaticPlatform>(Pair{50, 50}, env, "Host", 100, 5);
    host->installModule(std::make_shared<ConnectionModule>(1, 1, 100, 200));
    env->addToken(host);

    return env;
}

void testEliminateAllSuspects(int numTokens) {
    auto env = generateRandomPlatforms(100);
    auto ai = std::make_shared<AI>(env);
    ai->syncHosts();

    std::vector<std::shared_ptr<Suspect>> suspects;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 99);
    std::uniform_int_distribution<int> distribution2(0, 99);

    for (int i = 0; i < numTokens; ++i) {
        int randomIndex = distribution(generator);
        int randomIndex2 = distribution2(generator);
        suspects.push_back(std::make_shared<Suspect>(Pair{randomIndex, randomIndex2}, env, 3, 7));
    }

    for (const auto& suspect : suspects)
        env->addToken(suspect); // Assuming addToken is used for suspects as well

    auto start = std::chrono::high_resolution_clock::now();
    ai->eliminateAllSuspects();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> eliminateTime = end - start;
    std::cout << "Time " << eliminateTime.count() << " ms" << std::endl;
}

int main() {
    std::vector<int> suspectCounts = {100, 200, 300, 400, 500, 600}; // Varying number of suspects
    for (int count : suspectCounts) {
        testEliminateAllSuspects(count); // Call the new test function
    }
    return 0;
}