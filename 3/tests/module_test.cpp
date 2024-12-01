#include <gtest/gtest.h>
#include "modules/modules.h"
#include "objects/objects.h"

class ModuleTest : public ::testing::Test {
protected:
    Environment env;
    std::shared_ptr<Platform> platform;
    
    void SetUp() override {
        env.setSize(10, 10);
        platform = std::make_shared<StaticPlatform>(
            Pair{1, 1}, &env, "Test Platform", 100, 3
        );
        env.addToken(platform);
    }
};

TEST_F(ModuleTest, SensorModule) {
    auto sensor = std::make_shared<SensorModule>(
        1, 10, true, 5, SensorType::Optical
    );
    platform->installModule(sensor);
    
    auto suspect = std::make_shared<Suspect>(
        Pair{2, 2}, &env, 2, 3
    );
    env.addToken(suspect);
    
    Report report = sensor->getSurrounding();
    EXPECT_FALSE(report.objects.empty());
    auto visibleSuspect = sensor->getVisibleSuspect(report);
    EXPECT_NE(visibleSuspect, nullptr);
}

TEST_F(ModuleTest, WeaponModule) {
    auto weapon = std::make_shared<WeaponModule>(
        1, 10, true, 5, std::chrono::milliseconds(100)
    );
    platform->installModule(weapon);
    
    auto suspect = std::make_shared<Suspect>(
        Pair{2, 2}, &env, 2, 3
    );
    env.addToken(suspect);
    
    weapon->startCharging();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    weapon->update();
    
    EXPECT_TRUE(weapon->attack(Pair{2, 2}));
    EXPECT_EQ(env.getToken(Pair{2, 2}), nullptr);
}

TEST_F(ModuleTest, ConnectionModule) {
    auto conn1 = std::make_shared<ConnectionModule>(
        1, 10, true, 5, 5
    );
    auto conn2 = std::make_shared<ConnectionModule>(
        1, 10, true, 5, 5
    );
    
    auto platform2 = std::make_shared<StaticPlatform>(
        Pair{2, 2}, &env, "Platform2", 100, 3
    );
    env.addToken(platform2);
    
    platform->installModule(conn1);
    platform2->installModule(conn2);
    
    auto scannedModules = conn1->scanForModules();
    EXPECT_FALSE(scannedModules.empty());
    
    EXPECT_TRUE(conn1->establishConnection(conn2.get()));
    EXPECT_FALSE(conn1->getSessionList().empty());
    
    EXPECT_TRUE(conn1->closeConnection(conn2.get()));
    EXPECT_TRUE(conn1->getSessionList().empty());
}

TEST_F(ModuleTest, SensorModuleTypes) {
    auto opticalSensor = std::make_shared<SensorModule>(
        1, 10, true, 5, SensorType::Optical
    );
    auto xraySensor = std::make_shared<SensorModule>(
        1, 10, true, 5, SensorType::XRay
    );
    
    platform->installModule(opticalSensor);
    
    // Add obstacle between platform and suspect
    auto obstacle = std::make_shared<Obstacle>(Pair{1, 2}, &env);
    env.addToken(obstacle);
    auto suspect = std::make_shared<Suspect>(Pair{1, 3}, &env, 2, 3);
    env.addToken(suspect);
    
    // Optical sensor should not see through obstacles
    Report opticalReport = opticalSensor->getSurrounding();
    EXPECT_EQ(opticalSensor->getVisibleSuspect(opticalReport), nullptr);
    
    // Replace with XRay sensor
    platform->getModules().clear();
    platform->installModule(xraySensor);
    
    // XRay sensor should see through obstacles
    Report xrayReport = xraySensor->getSurrounding();
    EXPECT_NE(xraySensor->getVisibleSuspect(xrayReport), nullptr);
}

TEST_F(ModuleTest, WeaponModuleStates) {
    auto weapon = std::make_shared<WeaponModule>(
        1, 10, true, 5, std::chrono::milliseconds(100)
    );
    platform->installModule(weapon);
    
    // Test charging states
    EXPECT_FALSE(weapon->getIsCharging());
    EXPECT_FALSE(weapon->getIsCharged());
    
    weapon->startCharging();
    EXPECT_TRUE(weapon->getIsCharging());
    
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    weapon->update();
    EXPECT_TRUE(weapon->getIsCharged());
    
    // Test attack without target
    EXPECT_FALSE(weapon->attack(Pair{5, 5}));
}

TEST_F(ModuleTest, ConnectionModuleRouting) {
    auto conn1 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    auto conn2 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    auto conn3 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    
    auto platform2 = std::make_shared<StaticPlatform>(
        Pair{2, 2}, &env, "Platform2", 100, 3
    );
    auto platform3 = std::make_shared<StaticPlatform>(
        Pair{3, 3}, &env, "Platform3", 100, 3
    );
    
    env.addToken(platform2);
    env.addToken(platform3);
    
    platform->installModule(conn1);
    platform2->installModule(conn2);
    platform3->installModule(conn3);
    
    // Test route propagation
    conn1->establishConnection(conn2.get());
    conn2->establishConnection(conn3.get());
    
    auto routes = conn1->getRouteList();
    EXPECT_EQ(routes.size(), 2); // Should include both conn2 and conn3
    
    // Test route removal
    conn2->closeConnection(conn3.get());
    routes = conn1->getRouteList();
    EXPECT_EQ(routes.size(), 1); // Should only include conn2
} 