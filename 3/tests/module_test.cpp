#include <gtest/gtest.h>
#include "modules/modules.h"
#include "objects/objects.h"

class ModuleTest : public ::testing::Test {
protected:
    Environment env;
    std::shared_ptr<Platform> platform;
    
    void SetUp() override {
        env.setSize(10, 10);
        platform = std::make_shared<StaticPlatform>(Pair{1, 1}, &env, "Test Platform", 100, 3);
        env.addToken(platform);
    }
};

TEST_F(ModuleTest, SensorModule) {
    auto sensor = std::make_shared<SensorModule>(1, 10, true, 5, SensorType::Optical);
    platform->installModule(sensor);
    
    auto suspect = std::make_shared<Suspect>(Pair{3, 3}, &env, 2, 3);
    env.addToken(suspect);
    Report report = sensor->getSurrounding();
    EXPECT_FALSE(report.objects.empty());
    auto visibleSuspect = sensor->getVisibleSuspect(report);
    EXPECT_NE(visibleSuspect, nullptr);
}

TEST_F(ModuleTest, WeaponModule) {
    auto weapon = std::make_shared<WeaponModule>(1, 10, true, 5, std::chrono::milliseconds(100));
    platform->installModule(weapon);
    
    auto suspect = std::make_shared<Suspect>(Pair{2, 2}, &env, 2, 3);
    env.addToken(suspect);
    
    weapon->startCharging();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    weapon->update();
    
    EXPECT_TRUE(weapon->attack(Pair{2, 2}));
    EXPECT_EQ(env.getToken(Pair{2, 2}), nullptr);
}

TEST_F(ModuleTest, SensorModuleTypes) {
    auto opticalSensor = std::make_shared<SensorModule>(1, 10, true, 5, SensorType::Optical);
    auto xraySensor = std::make_shared<SensorModule>(1, 10, true, 5, SensorType::XRay);
    
    platform->installModule(opticalSensor);
    
    auto obstacle = std::make_shared<Obstacle>(Pair{1, 2}, &env);
    env.addToken(obstacle);
    auto suspect = std::make_shared<Suspect>(Pair{1, 3}, &env, 2, 3);
    env.addToken(suspect);
    
    Report opticalReport = opticalSensor->getSurrounding();
    EXPECT_EQ(opticalSensor->getVisibleSuspect(opticalReport), nullptr);
    
    platform->getModules().clear();
    platform->installModule(xraySensor);
    
    Report xrayReport = xraySensor->getSurrounding();
    EXPECT_EQ(xrayReport.objects.size(), 2);
}

TEST_F(ModuleTest, SensorModuleRange) {
    auto sensor = std::make_shared<SensorModule>(1, 10, true, 2, SensorType::Optical);
    platform->installModule(sensor);
    
    auto suspectInRange = std::make_shared<Suspect>(Pair{2, 2}, &env, 2, 3);
    env.addToken(suspectInRange);
    
    auto suspectOutOfRange = std::make_shared<Suspect>(Pair{4, 4}, &env, 2, 3);
    env.addToken(suspectOutOfRange);
    
    Report report = sensor->getSurrounding();
    EXPECT_EQ(report.objects.size(), 1);
}

TEST_F(ModuleTest, WeaponModuleStates) {
    auto weapon = std::make_shared<WeaponModule>(1, 10, true, 5, std::chrono::milliseconds(100));
    platform->installModule(weapon);
    
    EXPECT_FALSE(weapon->getIsCharging());
    EXPECT_FALSE(weapon->getIsCharged());
    
    weapon->startCharging();
    EXPECT_TRUE(weapon->getIsCharging());
    
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    weapon->update();
    EXPECT_TRUE(weapon->getIsCharged());
    
    EXPECT_FALSE(weapon->attack(Pair{5, 5}));
}

TEST_F(ModuleTest, WeaponModuleNoLineOfSight) {
    auto weapon = std::make_shared<WeaponModule>(1, 10, true, 5, std::chrono::milliseconds(100));
    platform->installModule(weapon);

    weapon->startCharging();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    weapon->update();

    auto obstacle = std::make_shared<Obstacle>(Pair{2, 2}, &env);
    env.addToken(obstacle);
    
    auto suspect = std::make_shared<Suspect>(Pair{3, 3}, &env, 2, 3);
    env.addToken(suspect);
    
    EXPECT_FALSE(weapon->attack(suspect->getPosition()));
}