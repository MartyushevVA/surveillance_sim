#include <gtest/gtest.h>
#include <random>
#include <future>
#include <thread>
#include "system/ai.h"
#include "objects/objects.h"
#include "modules/modules.h"

class ComplexScenariosTest : public ::testing::Test {
protected:
    std::shared_ptr<Environment> env;
    std::mt19937 gen{std::random_device{}()};
    
    void SetUp() override {
        env = std::make_shared<Environment>(Pair{50, 50});
    }
    
    Pair getRandomPosition() {
        std::uniform_int_distribution<> dis(0, 49);
        return {dis(gen), dis(gen)};
    }
    
    void createObstacleWall(Pair start, int length, bool vertical) {
        for(int i = 0; i < length; i++) {
            Pair pos = vertical ? 
                Pair{start.x, start.y + i} : 
                Pair{start.x + i, start.y};
            auto obstacle = std::make_shared<Obstacle>(pos, std::weak_ptr<Environment>(env));
            env->addToken(obstacle);
        }
    }
};

TEST_F(ComplexScenariosTest, MazeEscape) {
    createObstacleWall({10, 10}, 10, true);
    createObstacleWall({10, 10}, 10, false);
    createObstacleWall({20, 10}, 10, true);
    createObstacleWall({10, 20}, 10, false);
    
    auto suspect = std::make_shared<Suspect>(Pair{15, 15}, env, 5, 3);
    auto platform = std::make_shared<MobilePlatform>(Pair{11, 11}, env, "Hunter", 100, 3, 2);
    
    env->addToken(suspect);
    env->addToken(platform);
    
    for(int i = 0; i < 5; i++) {
        Pair oldPos = suspect->getPosition();
        Pair newPos = suspect->opponentBasedMove(platform->getPosition());
        EXPECT_TRUE(env->abilityToMove(oldPos, newPos));
        suspect->move(newPos);
        EXPECT_EQ(env->getToken(newPos), suspect);
    }
}

TEST_F(ComplexScenariosTest, NetworkPartitioning) {
    std::vector<std::shared_ptr<Platform>> platforms;
    
    for(int i = 0; i < 6; i++) {
        auto platform = std::make_shared<StaticPlatform>(
            Pair{10 + (i < 3 ? 0 : 20), 10 + i*5}, 
            std::weak_ptr<Environment>(env),
            "Platform" + std::to_string(i), 100, 3);
        auto connection = std::make_shared<ConnectionModule>(1, 10, 8, 5);
        platform->installModule(connection);
        platforms.push_back(platform);
        env->addToken(platform);
    }
    
    createObstacleWall({15, 0}, 50, true);
    
    for(auto& platform : platforms) {
        platform->findModuleOfType<ConnectionModule>()->update();
    }
    
    auto conn1 = platforms[0]->findModuleOfType<ConnectionModule>();
    auto conn4 = platforms[3]->findModuleOfType<ConnectionModule>();
    
    EXPECT_EQ(conn1->getRouteList().size(), 2);
    EXPECT_EQ(conn4->getRouteList().size(), 2);
}

TEST_F(ComplexScenariosTest, SensorNetworkOverlap) {
    std::vector<std::shared_ptr<Platform>> platforms;
    std::vector<std::shared_ptr<SensorModule>> sensors;
    
    for(int i = 0; i < 4; i++) {
        auto platform = std::make_shared<StaticPlatform>(
            Pair{20 + (i%2)*5, 20 + (i/2)*5}, 
            env, "Platform" + std::to_string(i), 100, 3);
        auto sensor = std::make_shared<SensorModule>(1, 10, 6, 
            i % 2 == 0 ? SensorType::Optical : SensorType::XRay);
        
        platform->installModule(sensor);
        platforms.push_back(platform);
        sensors.push_back(sensor);
        env->addToken(platform);
    }
    
    auto suspect = std::make_shared<Suspect>(Pair{22, 22}, env, 3, 2);
    env->addToken(suspect);
    
    std::vector<std::future<Report>> reports;
    for(auto& sensor : sensors) {
        reports.push_back(std::async(std::launch::async, [&]() {
            return sensor->getSurrounding();
        }));
    }
    
    for(auto& future : reports) {
        auto report = future.get();
        EXPECT_FALSE(report.objects.empty());
    }
}