#include <gtest/gtest.h>
#include "objects/objects.h"
#include "modules/modules.h"

class PlatformTest : public ::testing::Test {
protected:
    Environment env;
    std::shared_ptr<StaticPlatform> platform;
    
    void SetUp() override {
        env.setSize(10, 10);
        platform = std::make_shared<StaticPlatform>(Pair{1, 1}, &env, "Test Platform", 100, 3);
        env.addToken(platform);
    }
};

TEST_F(PlatformTest, InstallModule) {
    auto module = std::make_shared<SensorModule>(1, 10, true, 5, SensorType::Optical);

    EXPECT_NO_THROW(platform->installModule(module));
    EXPECT_EQ(platform->getModules().size(), 1);
}

TEST_F(PlatformTest, ModuleManagement) {
    auto module1 = std::make_shared<SensorModule>(1, 10, true, 5, SensorType::Optical);
    auto module2 = std::make_shared<WeaponModule>(1, 10, true, 5, std::chrono::milliseconds(100));
    auto module3 = std::make_shared<ConnectionModule>(2, 10, true, 5, 5);
    platform->installModule(module1);
    platform->installModule(module2);

    EXPECT_EQ(platform->getEnergyLevel(), 10);
    EXPECT_THROW(platform->installModule(module3), std::runtime_error);
}

TEST_F(PlatformTest, ModuleTypeSearch) {
    auto sensor = std::make_shared<SensorModule>(1, 10, true, 5, SensorType::Optical);
    platform->installModule(sensor);
    
    EXPECT_NE(platform->findModuleOfType<SensorModule>(), nullptr);
    EXPECT_EQ(platform->findModuleOfType<WeaponModule>(), nullptr);
}

TEST_F(PlatformTest, PlatformUpdate) {
    auto weapon = std::make_shared<WeaponModule>(1, 10, true, 5, std::chrono::milliseconds(100));
    platform->installModule(weapon);
    weapon->startCharging();
    platform->update();

    EXPECT_TRUE(weapon->getIsCharging());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    platform->update();

    EXPECT_FALSE(weapon->getIsCharging());
    EXPECT_TRUE(weapon->getIsCharged());
}

TEST_F(PlatformTest, PlatformBehavior) {
    auto mobilePlatform = std::make_shared<MobilePlatform>(Pair{2, 2}, &env, "Mobile", 100, 3, 2);
    env.addToken(mobilePlatform);

    mobilePlatform->setSpeed(3);
    EXPECT_EQ(mobilePlatform->getSpeed(), 3);

    platform->setSpeed(3);
    EXPECT_EQ(platform->getSpeed(), 0);

    auto newMobPos = Pair{1, 2};
    mobilePlatform->move(newMobPos);
    EXPECT_EQ(mobilePlatform->getPosition(), newMobPos);
}