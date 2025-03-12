#include <gtest/gtest.h>
#include <thread>
#include <random>
#include <future>
#include "objects/objects.h"
#include "modules/modules.h"

class MultithreadingPlatformTest : public ::testing::Test {
protected:
    std::shared_ptr<Environment> env;
    std::vector<std::shared_ptr<MobilePlatform>> platforms;

    void SetUp() override {
        env = std::make_shared<Environment>(Pair{50, 50});
        for (int i = 0; i < 5; ++i) {
            auto platform = std::make_shared<MobilePlatform>(
                getRandomPosition(), std::weak_ptr<Environment>(env), "Platform" + std::to_string(i), 100, 3, getRandomSpeed());
            platforms.push_back(platform);
            env->addToken(platform);
        }
    }

    Pair getRandomPosition() {
        std::uniform_int_distribution<> dis(0, 49);
        return {dis(gen), dis(gen)};
    }

    int getRandomSpeed() {
        std::uniform_int_distribution<> dis(1, 5);
        return dis(gen);
    }

    std::mt19937 gen{std::random_device{}()};
};

TEST_F(MultithreadingPlatformTest, ConcurrentPlatformUpdates) {
    std::vector<std::future<void>> futures;
    for (int i = 0; i < (int)platforms.size(); ++i) {
        futures.push_back(std::async(std::launch::async, [&, i]() {
            for (int j = 0; j < 10; ++j) {
                platforms[i]->iterate();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }));
    }

    for (auto& f : futures) f.get();

    for (const auto& platform : platforms) {
        EXPECT_NE(platform, nullptr);
    }
}

TEST_F(MultithreadingPlatformTest, ConcurrentSuspectMovements) {
    std::vector<std::shared_ptr<Suspect>> suspects;
    for (int i = 0; i < 5; ++i) {
        auto suspect = std::make_shared<Suspect>(
            getRandomPosition(), std::weak_ptr<Environment>(env), 3, getRandomSpeed());
        suspects.push_back(suspect);
        env->addToken(suspect);
    }

    std::vector<std::future<void>> futures;
    for (int i = 0; i < (int)suspects.size(); ++i) {
        futures.push_back(std::async(std::launch::async, [&, i]() {
            for (int j = 0; j < 10; ++j) {
                Pair newPos = suspects[i]->opponentBasedMove(platforms[i % platforms.size()]->getPosition());
                suspects[i]->move(newPos);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }));
    }

    for (auto& f : futures) f.get();

    for (const auto& suspect : suspects) {
        EXPECT_NE(suspect, nullptr);
    }
} 