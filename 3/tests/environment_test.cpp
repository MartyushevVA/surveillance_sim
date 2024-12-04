#include <gtest/gtest.h>
#include "system/environment.h"
#include "objects/objects.h"

class EnvironmentTest : public ::testing::Test {
protected:
    Environment env;
    void SetUp() override {
        env.setSize(10, 10);
    }
};

TEST_F(EnvironmentTest, AddToken) {
    auto obstacle = std::make_shared<Obstacle>(Pair{1, 1}, &env);
    EXPECT_NO_THROW(env.addToken(obstacle));
    EXPECT_EQ(env.getToken(Pair{1, 1}), obstacle);
}

TEST_F(EnvironmentTest, RemoveToken) {
    auto obstacle = std::make_shared<Obstacle>(Pair{1, 1}, &env);
    env.addToken(obstacle);
    env.removeToken(Pair{1, 1});
    EXPECT_EQ(env.getToken(Pair{1, 1}), nullptr);
}

TEST_F(EnvironmentTest, LineOfSight) {
    auto obstacle = std::make_shared<Obstacle>(Pair{1, 1}, &env);
    env.addToken(obstacle);
    
    EXPECT_FALSE(env.hasLineOfSight(Pair{0, 0}, Pair{2, 2}));
    
    EXPECT_TRUE(env.hasLineOfSight(Pair{0, 0}, Pair{0, 2}));
}

TEST_F(EnvironmentTest, OutOfBoundsOperations) {
    auto obstacle = std::make_shared<Obstacle>(Pair{11, 11}, &env);
    EXPECT_THROW(env.addToken(obstacle), std::invalid_argument);
    
    auto movable = std::make_shared<Suspect>(Pair{0, 0}, &env, 1, 1);
    env.addToken(movable);
    EXPECT_FALSE(env.abilityToMove(Pair{0, 0}, Pair{-1, -1}));
    EXPECT_FALSE(env.abilityToMove(Pair{0, 0}, Pair{10, 10}));
}

TEST_F(EnvironmentTest, DistanceCalculations) {
    EXPECT_EQ(env.getDistance(Pair{0, 0}, Pair{3, 4}), 5);
    
    EXPECT_DOUBLE_EQ(env.howFar(Pair{0, 0}, Pair{3, 0}, 3), 1.0);
    EXPECT_LT(env.howFar(Pair{0, 0}, Pair{1, 0}, 3), 1.0);
}

TEST_F(EnvironmentTest, ComplexLineOfSight) {
    auto obstacle1 = std::make_shared<Obstacle>(Pair{2, 2}, &env);
    auto obstacle2 = std::make_shared<Obstacle>(Pair{2, 3}, &env);
    auto obstacle3 = std::make_shared<Obstacle>(Pair{2, 4}, &env);
    env.addToken(obstacle1);
    env.addToken(obstacle2);
    env.addToken(obstacle3);
    
    EXPECT_FALSE(env.hasLineOfSight(Pair{1, 1}, Pair{3, 5}));
    EXPECT_TRUE(env.hasLineOfSight(Pair{1, 1}, Pair{1, 5}));
}

TEST_F(EnvironmentTest, TokenManagement) {
    auto obstacle1 = std::make_shared<Obstacle>(Pair{1, 1}, &env);
    auto obstacle2 = std::make_shared<Obstacle>(Pair{1, 1}, &env);
    
    env.addToken(obstacle1);
    EXPECT_THROW(env.addToken(obstacle2), std::invalid_argument);
    
    EXPECT_NO_THROW(env.removeToken(Pair{5, 5}));
    
    auto tokens = env.getTokens();
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens.begin()->second, obstacle1);
} 