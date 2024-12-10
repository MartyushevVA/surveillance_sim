#include <gtest/gtest.h>
#include <random>
#include <future>
#include <thread>
#include "system/ai.h"
#include "objects/objects.h"
#include "modules/modules.h"

class AITest : public ::testing::Test {
protected:
    Environment env;
    AI ai;
    std::mt19937 gen{std::random_device{}()};
    
    AITest() : env(), ai(&env) {
        env.setSize(30, 30);
    }
    
    Pair getRandomPosition() {
        std::uniform_int_distribution<> dis(0, 29);
        return {dis(gen), dis(gen)};
    }
    
    std::shared_ptr<Platform> createArmedPlatform(Pair pos, bool mobile = false) {
        std::shared_ptr<Platform> platform;
        if (mobile) {
            platform = std::make_shared<MobilePlatform>(pos, &env, "Armed Platform", 100, 3, 2);
        } else {
            platform = std::make_shared<StaticPlatform>(pos, &env, "Armed Platform", 100, 3, &ai);
        }
        
        auto sensor = std::make_shared<SensorModule>(1, 10, 8, SensorType::Optical);
        auto weapon = std::make_shared<WeaponModule>(1, 10, 8, std::chrono::milliseconds(50));
        auto connection = std::make_shared<ConnectionModule>(1, 10, 10, 5);
        
        platform->installModule(sensor);
        platform->installModule(weapon);
        platform->installModule(connection);

        weapon->startCharging();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        weapon->update();
        
        return platform;
    }
};

TEST_F(AITest, NetworkResilience) {
    auto platform1 = createArmedPlatform(Pair{10, 10});
    auto platform2 = createArmedPlatform(Pair{15, 15});
    auto platform3 = createArmedPlatform(Pair{20, 20});
    
    env.addToken(std::dynamic_pointer_cast<Placeholder>(platform1));
    env.addToken(std::dynamic_pointer_cast<Placeholder>(platform2));
    env.addToken(std::dynamic_pointer_cast<Placeholder>(platform3));
    
    ai.addStaticPlatform(std::static_pointer_cast<StaticPlatform>(platform1));
    ai.addStaticPlatform(std::static_pointer_cast<StaticPlatform>(platform2));
    ai.addStaticPlatform(std::static_pointer_cast<StaticPlatform>(platform3));
    
    auto suspect = std::make_shared<Suspect>(Pair{12, 12}, &env, 3, 2);
    env.addToken(std::dynamic_pointer_cast<Placeholder>(suspect));
    EXPECT_NE(env.getToken(suspect->getPosition()), nullptr);
    
    std::thread elimination([&]() {
        ai.eliminateAllSuspects();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    env.removeToken(platform2->getPosition());
    
    elimination.join();
    
    EXPECT_EQ(env.getToken(suspect->getPosition()), nullptr);
}

TEST_F(AITest, SimultaneousTargeting) {
    auto platform1 = createArmedPlatform(Pair{10, 10});
    auto platform2 = createArmedPlatform(Pair{14, 14});
    auto suspect = std::make_shared<Suspect>(Pair{12, 12}, &env, 3, 2);
    
    env.addToken(std::dynamic_pointer_cast<Placeholder>(platform1));
    env.addToken(std::dynamic_pointer_cast<Placeholder>(platform2));
    env.addToken(std::dynamic_pointer_cast<Placeholder>(suspect));
    EXPECT_NE(env.getToken(suspect->getPosition()), nullptr);
    
    ai.addStaticPlatform(std::dynamic_pointer_cast<StaticPlatform>(platform1));
    ai.addStaticPlatform(std::dynamic_pointer_cast<StaticPlatform>(platform2));
    
    std::future<void> attack1 = std::async(std::launch::async, [&]() {
        platform1->findModuleOfType<WeaponModule>()->attack(suspect->getPosition());
    });
    
    std::future<void> attack2 = std::async(std::launch::async, [&]() {
        platform2->findModuleOfType<WeaponModule>()->attack(suspect->getPosition());
    });
    
    attack1.get();
    attack2.get();
    
    EXPECT_EQ(env.getToken(suspect->getPosition()), nullptr);
}

TEST_F(AITest, EliminateAllSuspects_Multithreading) {
    auto platform1 = createArmedPlatform(Pair{10, 10}, false);
    auto platform2 = createArmedPlatform(Pair{15, 15}, false);
    auto suspect1 = std::make_shared<Suspect>(Pair{12, 12}, &env, 3, 2);
    auto suspect2 = std::make_shared<Suspect>(Pair{18, 18}, &env, 3, 2);
    
    env.addToken(std::dynamic_pointer_cast<Placeholder>(platform1));
    env.addToken(std::dynamic_pointer_cast<Placeholder>(platform2));
    env.addToken(std::dynamic_pointer_cast<Placeholder>(suspect1));
    env.addToken(std::dynamic_pointer_cast<Placeholder>(suspect2));
    
    ai.addStaticPlatform(std::dynamic_pointer_cast<StaticPlatform>(platform1));
    ai.addStaticPlatform(std::dynamic_pointer_cast<StaticPlatform>(platform2));
    
    EXPECT_NE(env.getToken(suspect1->getPosition()), nullptr);
    EXPECT_NE(env.getToken(suspect2->getPosition()), nullptr);

    std::thread eliminationThread([&]() {
        ai.eliminateAllSuspects();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    env.removeToken(suspect1->getPosition());

    eliminationThread.join();

    EXPECT_EQ(env.getToken(suspect1->getPosition()), nullptr);
    EXPECT_EQ(env.getToken(suspect2->getPosition()), nullptr);
}