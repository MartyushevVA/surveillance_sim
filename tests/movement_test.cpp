#include <gtest/gtest.h>
#include <random>
#include <thread>
#include <future>
#include "objects/objects.h"

class MovementTest : public ::testing::Test {
protected:
    std::shared_ptr<Environment> env;
    std::mt19937 gen{std::random_device{}()};
    
    void SetUp() override {
        env = std::make_shared<Environment>(Pair{50, 50});
    }
    
    Pair getRandomPosition() {
        std::uniform_int_distribution<> dis(0, 49);
        return {dis(gen), dis(gen)};
    }
    
    int getRandomSpeed() {
        std::uniform_int_distribution<> dis(1, 5);
        return dis(gen);
    }
};

TEST_F(MovementTest, SuspectEscapesBehavior) {
    auto suspect = std::make_shared<Suspect>(Pair{25, 25}, std::weak_ptr<Environment>(env), 3, getRandomSpeed());
    auto platform = std::make_shared<MobilePlatform>(Pair{20, 20}, std::weak_ptr<Environment>(env), "Hunter", 100, 3, getRandomSpeed());
    env->addToken(suspect);
    env->addToken(platform);

    double initialDistance = env->calculateDistance(suspect->getPosition(), platform->getPosition());
    
    for(int i = 0; i < 10; i++) {
        Pair newPos = suspect->opponentBasedMove(platform->getPosition());
        suspect->move(newPos);
        double newDistance = env->calculateDistance(suspect->getPosition(), platform->getPosition());
        EXPECT_GE(newDistance, initialDistance) << "Suspect should try to maintain or increase distance";
    }
}

TEST_F(MovementTest, SuspectEscapeFromMultiplePlatforms) {
    auto suspect = std::make_shared<Suspect>(Pair{25, 25}, std::weak_ptr<Environment>(env), 3, getRandomSpeed());
    env->addToken(suspect);
    
    std::vector<std::shared_ptr<MobilePlatform>> platforms;
    for(int i = 0; i < 3; i++) {
        auto platform = std::make_shared<MobilePlatform>(
            getRandomPosition(), std::weak_ptr<Environment>(env), "Hunter" + std::to_string(i), 100, 3, getRandomSpeed());
        platforms.push_back(platform);
        env->addToken(platform);
    }

    Pair initialPos = suspect->getPosition();
    suspect->move(suspect->opponentBasedMove(platforms[0]->getPosition()));
    
    EXPECT_NE(suspect->getPosition(), initialPos);
}

TEST_F(MovementTest, ConcurrentMovement) {
    std::vector<std::shared_ptr<Suspect>> suspects;
    std::vector<std::shared_ptr<MobilePlatform>> platforms;
    
    for(int i = 0; i < 5; i++) {
        suspects.push_back(std::make_shared<Suspect>(
            getRandomPosition(), std::weak_ptr<Environment>(env), 3, getRandomSpeed()));
        platforms.push_back(std::make_shared<MobilePlatform>(
            getRandomPosition(), std::weak_ptr<Environment>(env), "Platform" + std::to_string(i), 100, 3, getRandomSpeed()));
    }
    
    for(auto& s : suspects) env->addToken(s);
    for(auto& p : platforms) env->addToken(p);
    
    std::vector<std::future<void>> futures;
    for(int i = 0; i < 5; i++) {
        futures.push_back(std::async(std::launch::async, [&, i]() {
            for(int j = 0; j < 10; j++) {
                Pair newPos = suspects[i]->opponentBasedMove(platforms[i]->getPosition());
                suspects[i]->move(newPos);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }));
    }
    
    for(auto& f : futures) f.get();
    
    for(auto& s : suspects) {
        ASSERT_NE(s, nullptr);
        auto pos = s->getPosition();
        EXPECT_GE(pos.x, 0);
        EXPECT_GE(pos.y, 0);
        EXPECT_LT(pos.x, 50);
        EXPECT_LT(pos.y, 50);
    }
}

TEST_F(MovementTest, PursuitMovement) {
    auto mobilePlatform = std::make_shared<MobilePlatform>(getRandomPosition(), std::weak_ptr<Environment>(env), "Hunter", 100, 3, getRandomSpeed());
    Pair suspect = getRandomPosition();
    while (suspect == mobilePlatform->getPosition())
        suspect = getRandomPosition();
    Pair newPos = mobilePlatform->opponentBasedMove(suspect);
    int initialDistance = env->calculateDistance(mobilePlatform->getPosition(), suspect);
    int newDistance = env->calculateDistance(newPos, suspect);

    EXPECT_LE(newDistance, initialDistance);
}

TEST_F(MovementTest, AvoidanceMovement) {
    auto suspect = std::make_shared<Suspect>(getRandomPosition(), std::weak_ptr<Environment>(env), 100, getRandomSpeed());
    Pair mobilePlatform = getRandomPosition();
    while (mobilePlatform == suspect->getPosition())
        mobilePlatform = getRandomPosition();
    Pair newPos = suspect->opponentBasedMove(mobilePlatform);
    int initialDistance = env->calculateDistance(mobilePlatform, suspect->getPosition());
    int newDistance = env->calculateDistance(newPos, mobilePlatform);

    EXPECT_GE(newDistance, initialDistance);
}