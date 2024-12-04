#include <gtest/gtest.h>
#include "objects/objects.h"

class SuspectTest : public ::testing::Test {
protected:
    Environment env;
    std::shared_ptr<Suspect> suspect;
    
    void SetUp() override {
        env.setSize(10, 10);
        suspect = std::make_shared<Suspect>(Pair{5, 5}, &env, 2, 3);
        env.addToken(suspect);
    }
};

TEST_F(SuspectTest, DetectPredator) {
    auto platform = std::make_shared<StaticPlatform>(Pair{6, 6}, &env, "Predator", 100, 3);
    env.addToken(platform);
    
    Platform* detected = suspect->nearestPredatorWithinRange();
    EXPECT_NE(detected, nullptr);
    EXPECT_EQ(detected, platform.get());
}

TEST_F(SuspectTest, MovementBehavior) {
    Pair initialPos = suspect->getPosition();
    Pair newPos = suspect->calculateRandomMove();
    EXPECT_TRUE(env.abilityToMove(initialPos, newPos));
    
    auto predator = std::make_shared<StaticPlatform>(Pair{6, 6}, &env, "Predator", 100, 3);
    env.addToken(predator);
    
    Pair avoidancePos = suspect->calculateAvoidanceMove(predator->getPosition());
    int oldDist = env.getDistance(suspect->getPosition(), predator->getPosition());
    int newDist = env.getDistance(avoidancePos, predator->getPosition());
    EXPECT_GE(newDist, oldDist);
} 