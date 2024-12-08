#include <gtest/gtest.h>
#include <thread>

#include "modules/modules.h"
#include "objects/objects.h"

class ModuleTest : public ::testing::Test {
protected:
    Environment env;
    std::shared_ptr<Platform> platform;
    
    void SetUp() override {
        env.setSize(10, 10);
        platform = std::make_shared<StaticPlatform>(Pair{1, 1}, &env, "Test Platform", 100, 3, nullptr);
        env.addToken(std::dynamic_pointer_cast<Placeholder>(platform));
    }
};

TEST_F(ModuleTest, SensorModule) {
    auto sensor = std::make_shared<SensorModule>(1, 10, 5, SensorType::Optical);
    platform->installModule(sensor);
    sensor->update();
    auto suspect = std::make_shared<Suspect>(Pair{3, 3}, &env, 2, 3);
    env.addToken(suspect);
    Report report = sensor->getSurrounding();
    EXPECT_FALSE(report.objects.empty());
    auto visibleSuspect = sensor->getNearestVisibleOpponent();
    EXPECT_NE(visibleSuspect, nullptr);
}

TEST_F(ModuleTest, WeaponModule) {
    auto weapon = std::make_shared<WeaponModule>(1, 10, 5, std::chrono::milliseconds(100));
    platform->installModule(weapon);
    
    auto suspect = std::make_shared<Suspect>(Pair{2, 2}, &env, 2, 3);
    env.addToken(suspect);
    
    weapon->startCharging();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    weapon->update();
    
    EXPECT_TRUE(weapon->attack(Pair{2, 2}));
    EXPECT_EQ(env.getToken(Pair{2, 2}), nullptr);
}

TEST_F(ModuleTest, ConnectionModule) {
    auto conn1 = std::make_shared<ConnectionModule>(1, 10, 5, 5);
    auto conn2 = std::make_shared<ConnectionModule>(1, 10, 5, 5);
    
    auto platform2 = std::make_shared<MobilePlatform>(Pair{2, 2}, &env, "Platform2", 100, 3, 2);
    env.addToken(platform2);
    
    platform->installModule(conn1);
    platform2->installModule(conn2);
    
    auto scannedModules = conn1->scanForModules(Pair{1, 1});
    EXPECT_EQ(scannedModules[0], conn2.get());
    EXPECT_TRUE(conn1->establishConnection(conn2.get()));
    EXPECT_FALSE(conn1->getSessionList().empty());

    EXPECT_TRUE(conn1->closeConnection(conn2.get()));
    EXPECT_TRUE(conn1->getSessionList().empty());
}

TEST_F(ModuleTest, SensorModuleTypes) {
    auto opticalSensor = std::make_shared<SensorModule>(1, 10, 5, SensorType::Optical);
    auto xraySensor = std::make_shared<SensorModule>(1, 10, 5, SensorType::XRay);
    
    platform->installModule(opticalSensor);
    
    auto obstacle = std::make_shared<Obstacle>(Pair{1, 2}, &env);
    env.addToken(obstacle);
    auto suspect = std::make_shared<Suspect>(Pair{1, 3}, &env, 2, 3);
    env.addToken(suspect);
    
    Report opticalReport = opticalSensor->getSurrounding();
    EXPECT_EQ(opticalSensor->getNearestVisibleOpponent(), nullptr);
    
    platform->getModules().clear();
    platform->installModule(xraySensor);
    
    Report xrayReport = xraySensor->getSurrounding();
    EXPECT_EQ(xrayReport.objects.size(), 2);
}

TEST_F(ModuleTest, SensorModuleRange) {
    auto sensor = std::make_shared<SensorModule>(1, 10, 5, SensorType::Optical);
    platform->installModule(sensor);
    
    auto suspectInRange = std::make_shared<Suspect>(Pair{2, 2}, &env, 2, 3);
    env.addToken(suspectInRange);
    
    auto suspectOutOfRange = std::make_shared<Suspect>(Pair{4, 4}, &env, 2, 3);
    env.addToken(suspectOutOfRange);
    
    Report report = sensor->getSurrounding();
    EXPECT_EQ(report.objects.size(), 1);
}

TEST_F(ModuleTest, WeaponModuleStates) {
    auto weapon = std::make_shared<WeaponModule>(1, 10, 5, std::chrono::milliseconds(100));
    platform->installModule(weapon);
    
    EXPECT_TRUE(weapon->getIsCharging());
    EXPECT_FALSE(weapon->getIsCharged());
    
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    weapon->update();
    EXPECT_TRUE(weapon->getIsCharged());
    
    EXPECT_FALSE(weapon->attack(Pair{5, 5}));
}

TEST_F(ModuleTest, WeaponModuleNoLineOfSight) {
    auto weapon = std::make_shared<WeaponModule>(1, 10, 5, std::chrono::milliseconds(100));
    platform->installModule(weapon);

    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    weapon->update();

    auto obstacle = std::make_shared<Obstacle>(Pair{2, 2}, &env);
    env.addToken(obstacle);
    
    auto suspect = std::make_shared<Suspect>(Pair{3, 3}, &env, 2, 3);
    env.addToken(suspect);
    
    EXPECT_FALSE(weapon->attack(suspect->getPosition()));
}

TEST_F(ModuleTest, SuicideTest) {
    Environment env;
    env.setSize(10, 10);
    auto platform = std::make_shared<StaticPlatform>(Pair{1, 1}, &env, "Test Platform", 100, 3, nullptr);
    env.addToken(platform);
    auto robot1 = std::make_shared<MobilePlatform>(Pair{3, 3}, &env, "Robot1", 100, 3, 3);
    env.addToken(robot1);
    auto robot2 = std::make_shared<MobilePlatform>(Pair{5, 5}, &env, "Robot2", 100, 3, 3);
    env.addToken(robot2);

    auto conn1 = std::make_shared<ConnectionModule>(1, 10, 3, 3);
    auto conn2 = std::make_shared<ConnectionModule>(1, 10, 3, 3);
    auto conn3 = std::make_shared<ConnectionModule>(1, 10, 3, 3);
    platform->installModule(conn1);
    robot1->installModule(conn2);
    robot2->installModule(conn3);

    conn1->establishConnection(conn2.get());
    conn2->establishConnection(conn3.get());

    EXPECT_FALSE(conn3->isSafeForSystem(Pair{8, 8}));
}

/*TEST_F(ModuleTest, ConnectionModuleRouting) {
    auto conn1 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    auto conn2 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    auto conn3 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    
    auto platform2 = std::make_shared<MobilePlatform>(Pair{2, 2}, &env, "Platform2", 100, 3, 2);
    auto platform3 = std::make_shared<MobilePlatform>(Pair{3, 3}, &env, "Platform3", 100, 3, 2);
    
    env.addToken(platform2);
    env.addToken(platform3);
    
    platform->installModule(conn1);
    platform2->installModule(conn2);
    platform3->installModule(conn3);
    
    conn1->closeConnection(conn2.get());
    conn1->closeConnection(conn3.get());
    conn2->closeConnection(conn3.get());

    EXPECT_EQ(conn1->getRouteList().size(), 0);
    EXPECT_EQ(conn2->getRouteList().size(), 0);
    EXPECT_EQ(conn3->getRouteList().size(), 0);

    conn1->establishConnection(conn2.get());
    conn1->establishConnection(conn3.get());

    auto routes = conn1->getRouteList();
    for (auto route : routes)
        std::cout << route.destination->getHost()->getPosition().x << " " << route.destination->getHost()->getPosition().y << std::endl;
    EXPECT_EQ(routes.size(), 2);
    
    routes = conn1->getRouteList();
    EXPECT_EQ(routes.size(), 1);
}*/