#include <gtest/gtest.h>
#include <random>
#include <future>
#include <thread>
#include "system/ai.h"
#include "objects/objects.h"
#include "modules/modules.h"

class AITest : public ::testing::Test {
protected:
    std::shared_ptr<Environment> env;
    AI ai;
    std::mt19937 gen{std::random_device{}()};
    
    AITest() : env(std::make_shared<Environment>(Pair{30, 30})), ai(env) {}
    
    Pair getRandomPosition() {
        std::uniform_int_distribution<> dis(0, 29);
        return {dis(gen), dis(gen)};
    }
    
    std::shared_ptr<Platform> createArmedPlatform(Pair pos, bool mobile = false) {
        std::shared_ptr<Platform> platform;
        if (mobile) {
            platform = std::make_shared<MobilePlatform>(pos, std::weak_ptr<Environment>(env), "Armed Platform", 100, 3, 2);
        } else {
            platform = std::make_shared<StaticPlatform>(pos, std::weak_ptr<Environment>(env), "Armed Platform", 100, 3);
        }
        
        auto sensor = std::make_shared<SensorModule>(1, 10, 8, SensorType::Optical);
        auto weapon = std::make_shared<WeaponModule>(1, 10, 8, std::chrono::milliseconds(50));
        auto connection = std::make_shared<ConnectionModule>(1, 10, 10, 5);
        
        platform->installModule(sensor);
        platform->installModule(weapon);
        platform->installModule(connection);

        weapon->startCharging();
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
        weapon->update();
        
        return platform;
    }

    std::shared_ptr<Platform> createNotArmedPlatform(Pair pos, bool mobile = false) {
        std::shared_ptr<Platform> platform;
        if (mobile) {
            platform = std::make_shared<MobilePlatform>(pos, std::weak_ptr<Environment>(env), "Not Armed Platform", 100, 3, 2);
        } else {
            platform = std::make_shared<StaticPlatform>(pos, std::weak_ptr<Environment>(env), "Not Armed Platform", 100, 3);
        }
        auto sensor = std::make_shared<SensorModule>(1, 10, 8, SensorType::Optical);
        auto connection = std::make_shared<ConnectionModule>(1, 10, 10, 5);
        
        platform->installModule(sensor);
        platform->installModule(connection);

        return platform;
    }
};

TEST_F(AITest, NetworkResilience) {
    auto platform1 = createArmedPlatform(Pair{10, 10}, true);
    auto platform2 = createArmedPlatform(Pair{15, 15});
    
    env->addToken(std::dynamic_pointer_cast<Placeholder>(platform1));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(platform2));
    
    auto Ai = std::make_shared<AI>(env);
    Ai->syncHosts();

    auto suspect = std::make_shared<Suspect>(Pair{12, 12}, env, 3, 2);
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect));
    
    for (int i = 0; i < 10; i++) {
        EXPECT_NO_THROW(Ai->eliminateAllSuspects());
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    EXPECT_EQ(Ai->getSuspects().size(), 0);
}

TEST_F(AITest, SimultaneousTargeting) {
    auto platform1 = createArmedPlatform(Pair{10, 10});
    auto platform2 = createArmedPlatform(Pair{14, 14});
    auto suspect = std::make_shared<Suspect>(Pair{12, 12}, env, 3, 2);
    
    env->addToken(std::dynamic_pointer_cast<Placeholder>(platform1));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(platform2));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect));
    EXPECT_NE(env->getToken(suspect->getPosition()), nullptr);
    
    auto Ai = std::make_shared<AI>(env);
    Ai->syncHosts();
    
    std::future<void> attack1 = std::async(std::launch::async, [&]() {
        platform1->findModuleOfType<WeaponModule>()->attack(suspect->getPosition());
    });
    
    std::future<void> attack2 = std::async(std::launch::async, [&]() {
        platform2->findModuleOfType<WeaponModule>()->attack(suspect->getPosition());
    });
    
    attack1.get();
    attack2.get();
    
    EXPECT_EQ(env->getToken(suspect->getPosition()), nullptr);
}

TEST_F(AITest, SuspectsExistBeforeElimination) {
    Pair suspect1Pos = {12, 12};
    Pair suspect2Pos = {18, 18};
    auto suspect1 = std::make_shared<Suspect>(suspect1Pos, env, 3, 2);
    auto suspect2 = std::make_shared<Suspect>(suspect2Pos, env, 3, 2);
    
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect1));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect2));

    EXPECT_NE(env->getToken(suspect1Pos), nullptr);
    EXPECT_NE(env->getToken(suspect2Pos), nullptr);
}

TEST_F(AITest, SuspectsNonExistAfterElimination) {
    Pair suspect1Pos = {12, 12};
    Pair suspect2Pos = {18, 18};
    auto suspect1 = std::make_shared<Suspect>(suspect1Pos, env, 3, 2);
    auto suspect2 = std::make_shared<Suspect>(suspect2Pos, env, 3, 2);
    auto statucPlatforms = std::make_shared<StaticPlatform>(Pair{13, 12}, std::weak_ptr<Environment>(env), "Static Platform", 100, 3);
    auto sensor = std::make_shared<SensorModule>(1, 10, 30, SensorType::Optical);
    auto weapon = std::make_shared<WeaponModule>(1, 10, 30, std::chrono::milliseconds(50));
    auto connection = std::make_shared<ConnectionModule>(1, 10, 30, 5);

    statucPlatforms->installModule(sensor);
    statucPlatforms->installModule(weapon);
    statucPlatforms->installModule(connection);

    env->addToken(std::dynamic_pointer_cast<Placeholder>(statucPlatforms));

    auto Ai = std::make_shared<AI>(env);
    Ai->syncHosts();

    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect1));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect2));
    
    for (int i = 0; i < 10; i++) {
        EXPECT_NO_THROW(Ai->eliminateAllSuspects());
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
    }

    EXPECT_EQ(env->getToken(suspect1Pos), nullptr);
    EXPECT_EQ(env->getToken(suspect2Pos), nullptr);
}

TEST_F(AITest, MultipleEliminations) {
    Pair suspectPos = {12, 12};
    auto suspect = std::make_shared<Suspect>(suspectPos, env, 3, 2);
    auto statucPlatforms = std::make_shared<StaticPlatform>(Pair{13, 12}, std::weak_ptr<Environment>(env), "Static Platform", 100, 3);
    auto staticPlatforms2 = std::make_shared<StaticPlatform>(Pair{13, 18}, std::weak_ptr<Environment>(env), "Static Platform", 100, 3);
    auto sensor1 = std::make_shared<SensorModule>(1, 10, 30, SensorType::Optical);
    auto weapon1 = std::make_shared<WeaponModule>(1, 10, 30, std::chrono::milliseconds(50));
    auto connection1 = std::make_shared<ConnectionModule>(1, 10, 30, 5);
    auto sensor2 = std::make_shared<SensorModule>(1, 10, 30, SensorType::Optical);
    auto weapon2 = std::make_shared<WeaponModule>(1, 10, 30, std::chrono::milliseconds(50));
    auto connection2 = std::make_shared<ConnectionModule>(1, 10, 30, 5);

    statucPlatforms->installModule(sensor1);
    statucPlatforms->installModule(weapon1);
    statucPlatforms->installModule(connection1);
    weapon1->startCharging();
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    weapon1->update();
    staticPlatforms2->installModule(sensor2);
    staticPlatforms2->installModule(weapon2);
    staticPlatforms2->installModule(connection2);
    weapon1->startCharging();
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    weapon1->update();

    env->addToken(std::dynamic_pointer_cast<Placeholder>(statucPlatforms));

    auto Ai = std::make_shared<AI>(env);
    Ai->syncHosts();
    
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect));

    EXPECT_NO_THROW(Ai->eliminateAllSuspects());
    EXPECT_EQ(env->getToken(suspectPos), nullptr);

    EXPECT_NO_THROW(Ai->eliminateAllSuspects());
    EXPECT_EQ(env->getToken(suspectPos), nullptr);
}