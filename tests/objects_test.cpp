#include <gtest/gtest.h>
#include <random>
#include <memory>
#include <thread>
#include "system/ai.h"
#include "objects/objects.h"
#include "modules/modules.h"

class ObjectsTest : public ::testing::Test {
protected:
    std::shared_ptr<Environment> env;
    AI ai;
    std::mt19937 gen{std::random_device{}()};

    ObjectsTest() : env(std::make_shared<Environment>(Pair{30, 30})), ai(env) {}

    Pair getRandomPosition() {
        std::uniform_int_distribution<> dis(0, 29);
        return {dis(gen), dis(gen)};
    }

    std::shared_ptr<Platform> createPlatform(Pair pos, bool mobile = false) {
        std::shared_ptr<Platform> platform;
        if (mobile) {
            platform = std::make_shared<MobilePlatform>(pos, std::weak_ptr<Environment>(env), "Mobile Platform", 100, 3, 2);
        } else {
            platform = std::make_shared<StaticPlatform>(pos, std::weak_ptr<Environment>(env), "Static Platform", 100, 3);
        }

        auto connection = std::make_shared<ConnectionModule>(1, 10, 10, 5);
        platform->installModule(connection);
        auto sensor = std::make_shared<SensorModule>(1, 10, 20, SensorType::XRay);
        platform->installModule(sensor);
        auto weapon = std::make_shared<WeaponModule>(1, 10, 10, std::chrono::milliseconds(100));
        platform->installModule(weapon);
        return platform;
    }

    std::shared_ptr<Suspect> createSuspect(Pair pos) {
        return std::make_shared<Suspect>(pos, env, 3, 2);
    }
};

TEST_F(ObjectsTest, MobilePlatformIterate_WithTarget) {
    auto staticPlatform = createPlatform(Pair{1, 1}, false);
    auto mobilePlatform = createPlatform(Pair{5, 5}, true);
    auto suspect = createSuspect(Pair{10, 10});
    
    env->addToken(std::dynamic_pointer_cast<Placeholder>(staticPlatform));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(mobilePlatform));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect));
    
    auto connection = staticPlatform->findModuleOfType<ConnectionModule>();
    connection->establishConnection(mobilePlatform->findModuleOfType<ConnectionModule>());

    auto Ai = std::make_shared<AI>(env);
    Ai->syncHosts();    

    EXPECT_NE(mobilePlatform->getPosition().x * mobilePlatform->getPosition().y, 100);

    suspect->iterate();
    EXPECT_NE(suspect->getPosition().x * suspect->getPosition().y, 400);
}

TEST_F(ObjectsTest, StaticPlatformIterate_WithTarget) {
    auto staticPlatform = createPlatform(Pair{10, 10}, false);
    auto suspect = createSuspect(Pair{12, 12});
    env->addToken(std::dynamic_pointer_cast<Placeholder>(staticPlatform));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect));

    staticPlatform->iterate();
    EXPECT_EQ(staticPlatform->getPosition().x, 10);
    EXPECT_EQ(staticPlatform->getPosition().y, 10);
}

TEST_F(ObjectsTest, EliminateAllSuspects) {
    auto platform = createPlatform(Pair{10, 10}, false);
    auto suspect1 = createSuspect(Pair{12, 12});
    auto suspect2 = createSuspect(Pair{15, 15});
    env->addToken(std::dynamic_pointer_cast<Placeholder>(platform));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect1));
    env->addToken(std::dynamic_pointer_cast<Placeholder>(suspect2));

    auto Ai = std::make_shared<AI>(env);
    Ai->syncHosts();

    EXPECT_NE(env->getToken(suspect1->getPosition()), nullptr);
    EXPECT_NE(env->getToken(suspect2->getPosition()), nullptr);

    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    platform->findModuleOfType<WeaponModule>()->update();
    Ai->eliminateAllSuspects();
    platform->findModuleOfType<WeaponModule>()->startCharging();

    EXPECT_EQ(env->getToken(suspect1->getPosition()), nullptr);
    EXPECT_NE(env->getToken(suspect2->getPosition()), nullptr);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    platform->findModuleOfType<WeaponModule>()->update();
    Ai->eliminateAllSuspects();

    EXPECT_EQ(env->getToken(suspect1->getPosition()), nullptr);
    EXPECT_EQ(env->getToken(suspect2->getPosition()), nullptr);
}