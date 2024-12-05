#include <gtest/gtest.h>
#include "modules/modules.h"
#include "objects/objects.h"


TEST(ConnectionTest, ConnectionModule) {
    Environment env;
    std::shared_ptr<Platform> platform;
    env.setSize(10, 10);
    platform = std::make_shared<StaticPlatform>(Pair{1, 1}, &env, "Test Platform", 100, 3);
    env.addToken(platform);

    auto conn1 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    auto conn2 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    
    auto platform2 = std::make_shared<MobilePlatform>(Pair{2, 2}, &env, "Platform2", 100, 3, 2);
    env.addToken(platform2);
    
    platform->installModule(conn1);
    platform2->installModule(conn2);

    EXPECT_NO_THROW(conn1->update());
    
    auto scannedModules = conn1->scanForModules();
    EXPECT_TRUE(scannedModules[0] == conn2.get());
    
    EXPECT_TRUE(conn1->closeConnection(conn2.get()));
    EXPECT_TRUE(conn1->getSessionList().empty());

    EXPECT_TRUE(conn1->establishConnection(conn2.get()));
    EXPECT_FALSE(conn1->getSessionList().empty());
}

TEST(ConnectionTest, SimpleConnectionModuleRouting) {
    Environment env;
    std::shared_ptr<Platform> platform;
    env.setSize(10, 10);
    platform = std::make_shared<StaticPlatform>(Pair{1, 1}, &env, "Test Platform", 100, 3);
    env.addToken(platform);

    auto conn1 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    auto conn2 = std::make_shared<ConnectionModule>(1, 10, true, 5, 5);
    
    auto platform2 = std::make_shared<MobilePlatform>(Pair{2, 2}, &env, "Platform2", 100, 3, 2);
    
    env.addToken(platform2);
    
    platform->installModule(conn1);
    platform2->installModule(conn2);
    
    EXPECT_EQ(conn1->getRouteList().size(), 1);
    EXPECT_EQ(conn2->getRouteList().size(), 1);

    EXPECT_EQ(conn1->getRouteList()[0].destination, conn2.get());
    EXPECT_EQ(conn1->getRouteList()[0].gate, conn2.get());
    EXPECT_EQ(conn2->getRouteList()[0].destination, conn1.get());
    EXPECT_EQ(conn2->getRouteList()[0].gate, conn1.get());
}

TEST(ConnectionTest, ComplexConnectionModuleRouting) {
    Environment env1(20, 20);
    auto mobilePlatform1 = std::make_shared<MobilePlatform>(Pair{5, 5}, &env1, "AgileMobilePlatform 1", 100, 3, 2);
    auto mobilePlatform2 = std::make_shared<MobilePlatform>(Pair{5, 15}, &env1, "AgileMobilePlatform 2", 100, 3, 2);
    auto staticPlatform = std::make_shared<StaticPlatform>(Pair{10, 5}, &env1, "StaticPlatform", 100, 3);
    env1.addToken(staticPlatform);
    env1.addToken(mobilePlatform1);
    env1.addToken(mobilePlatform2);

    auto conn1 = std::make_shared<ConnectionModule>(1, 10, true, 10, 5);
    auto conn2 = std::make_shared<ConnectionModule>(1, 10, true, 10, 5);
    auto conn3 = std::make_shared<ConnectionModule>(1, 10, true, 10, 5);

    mobilePlatform1->installModule(conn1);
    mobilePlatform2->installModule(conn2);
    staticPlatform->installModule(conn3);

    auto secondRouteList = conn2->getRouteList();

    EXPECT_EQ(secondRouteList.size(), 2);
    EXPECT_EQ(secondRouteList[0].gate, conn1.get());
    EXPECT_EQ(secondRouteList[1].gate, conn1.get());
}

TEST(ConnectionTest, IsSafeForSystem) {
    Environment env1(50, 50);
    auto mobilePlatform1 = std::make_shared<MobilePlatform>(Pair{5, 5}, &env1, "AgileMobilePlatform 1", 100, 3, 2);
    auto mobilePlatform2 = std::make_shared<MobilePlatform>(Pair{5, 15}, &env1, "AgileMobilePlatform 2", 100, 3, 2);
    auto staticPlatform = std::make_shared<StaticPlatform>(Pair{10, 5}, &env1, "StaticPlatform", 100, 3);
    env1.addToken(staticPlatform);
    env1.addToken(mobilePlatform1);
    env1.addToken(mobilePlatform2);

    auto conn1 = std::make_shared<ConnectionModule>(1, 10, true, 10, 5);
    auto conn2 = std::make_shared<ConnectionModule>(1, 10, true, 10, 5);
    auto conn3 = std::make_shared<ConnectionModule>(1, 10, true, 10, 5);

    mobilePlatform1->installModule(conn1);
    mobilePlatform2->installModule(conn2);
    staticPlatform->installModule(conn3);

    auto secondRouteList = conn2->getRouteList();

    EXPECT_FALSE(conn1->isSafeForSystem({3, 3}));
    EXPECT_TRUE(conn1->isSafeForSystem({10, 15}));
}