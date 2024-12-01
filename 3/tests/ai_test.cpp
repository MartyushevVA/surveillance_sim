#include <gtest/gtest.h>
#include "system/ai.h"
#include "objects/objects.h"
#include "modules/modules.h"

class AITest : public ::testing::Test {
protected:
    Environment env;
    AI ai;
    std::shared_ptr<StaticPlatform> staticPlatform;
    std::shared_ptr<Suspect> suspect;
    
    AITest() : env(), ai(&env) {
        env.setSize(10, 10);
    }
    
    void SetUp() override {
        // Create static platform with sensor and weapon
        staticPlatform = std::make_shared<StaticPlatform>(
            Pair{1, 1}, &env, "Base", 100, 3
        );
        
        auto sensor = std::make_shared<SensorModule>(
            1, 10, true, 5, SensorType::Optical
        );
        auto weapon = std::make_shared<WeaponModule>(
            1, 10, true, 5, std::chrono::milliseconds(100)
        );
        
        staticPlatform->installModule(sensor);
        staticPlatform->installModule(weapon);
        env.addToken(staticPlatform);
        
        // Add suspect
        suspect = std::make_shared<Suspect>(
            Pair{3, 3}, &env, 2, 3
        );
        env.addToken(suspect);
        
        ai.addStaticPlatform(staticPlatform.get());
    }
};

TEST_F(AITest, EliminateSuspect) {
    EXPECT_NE(env.getToken(Pair{3, 3}), nullptr);
    ai.eliminateAllSuspects();
    EXPECT_EQ(env.getToken(Pair{3, 3}), nullptr);
}

TEST_F(AITest, NetworkFormation) {
    auto platform2 = std::make_shared<StaticPlatform>(
        Pair{2, 2}, &env, "Platform2", 100, 3
    );
    auto connection1 = std::make_shared<ConnectionModule>(
        1, 10, true, 5, 5
    );
    auto connection2 = std::make_shared<ConnectionModule>(
        1, 10, true, 5, 5
    );
    
    staticPlatform->installModule(connection1);
    platform2->installModule(connection2);
    env.addToken(platform2);
    ai.addStaticPlatform(platform2.get());
    
    ai.eliminateAllSuspects();
    // Verify that connections are established
    EXPECT_FALSE(connection1->getSessionList().empty());
} 