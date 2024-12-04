#include <gtest/gtest.h>
#include "objects/objects.h"

class MovementTest : public ::testing::Test {
protected:
    Environment env;
    std::shared_ptr<MobilePlatform> mobilePlatform;
    std::shared_ptr<Suspect> suspect;
    
    void SetUp() override {
        env.setSize(10, 10);
        mobilePlatform = std::make_shared<MobilePlatform>(Pair{1, 1}, &env, "Mobile Platform", 100, 3, 2);
        suspect = std::make_shared<Suspect>(Pair{5, 5}, &env, 2, 3);
        env.addToken(mobilePlatform);
        env.addToken(suspect);
    }
};

TEST_F(MovementTest, PursuitMovement) {
    Pair target = suspect->getPosition();
    Pair newPos = mobilePlatform->calculatePursuitMove(target);
    
    int oldDist = env.getDistance(mobilePlatform->getPosition(), target);
    int newDist = env.getDistance(newPos, target);
    EXPECT_LE(newDist, oldDist);
}

TEST_F(MovementTest, AvoidanceMovement) {
    Pair threat = mobilePlatform->getPosition();
    Pair newPos = suspect->calculateAvoidanceMove(threat);
    
    int oldDist = env.getDistance(suspect->getPosition(), threat);
    int newDist = env.getDistance(newPos, threat);
    EXPECT_GE(newDist, oldDist);
}