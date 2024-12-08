#include <gtest/gtest.h>
#include <random>
#include <future>
#include "system/ai.h"
#include "objects/objects.h"
#include "modules/modules.h"

class ComplexScenariosTest : public ::testing::Test {
protected:
    Environment env;
    std::mt19937 gen{std::random_device{}()};
    
    void SetUp() override {
        env.setSize(50, 50);
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
            auto obstacle = std::make_shared<Obstacle>(pos, &env);
            env.addToken(obstacle);
        }
    }
};

TEST_F(ComplexScenariosTest, MazeEscape) {
    // Создаем лабиринт из препятствий
    createObstacleWall({10, 10}, 10, true);
    createObstacleWall({10, 10}, 10, false);
    createObstacleWall({20, 10}, 10, true);
    createObstacleWall({10, 20}, 10, false);
    
    auto suspect = std::make_shared<Suspect>(Pair{15, 15}, &env, 5, 3);
    auto platform = std::make_shared<MobilePlatform>(Pair{11, 11}, &env, "Hunter", 100, 3, 2);
    
    env.addToken(suspect);
    env.addToken(platform);
    
    // Проверяем, что подозреваемый находит путь к выходу
    for(int i = 0; i < 20; i++) {
        Pair oldPos = suspect->getPosition();
        suspect->move(suspect->opponentBasedMove(platform->getPosition()));
        
        // Проверяем, что движение было возможным
        EXPECT_TRUE(env.abilityToMove(oldPos, suspect->getPosition()));
    }
}

TEST_F(ComplexScenariosTest, NetworkPartitioning) {
    std::vector<std::shared_ptr<Platform>> platforms;
    
    // Создаем две группы платформ, разделенные стеной
    for(int i = 0; i < 6; i++) {
        auto platform = std::make_shared<StaticPlatform>(
            Pair{10 + (i < 3 ? 0 : 20), 10 + i*5}, 
            &env, "Platform" + std::to_string(i), 100, 3);
        auto connection = std::make_shared<ConnectionModule>(1, 10, true, 8, 5);
        platform->installModule(connection);
        platforms.push_back(platform);
        env.addToken(platform);
    }
    
    // Создаем разделяющую стену
    createObstacleWall({15, 0}, 50, true);
    
    // Обновляем соединения
    for(auto& platform : platforms) {
        platform->findModuleOfType<ConnectionModule>()->update();
    }
    
    // Проверяем, что образовались две отдельные сети
    auto conn1 = platforms[0]->findModuleOfType<ConnectionModule>();
    auto conn4 = platforms[3]->findModuleOfType<ConnectionModule>();
    
    EXPECT_EQ(conn1->getSessionList().size(), 2);  // Только две другие платформы в первой группе
    EXPECT_EQ(conn4->getSessionList().size(), 2);  // Только две другие платформы во второй группе
}

TEST_F(ComplexScenariosTest, ConcurrentEnvironmentModification) {
    std::mutex envMutex;
    std::vector<std::future<void>> futures;
    
    // Параллельно добавляем и удаляем объекты
    for(int i = 0; i < 10; i++) {
        futures.push_back(std::async(std::launch::async, [&]() {
            for(int j = 0; j < 5; j++) {
                auto pos = getRandomPosition();
                {
                    std::lock_guard<std::mutex> lock(envMutex);
                    auto obstacle = std::make_shared<Obstacle>(pos, &env);
                    env.addToken(obstacle);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                {
                    std::lock_guard<std::mutex> lock(envMutex);
                    env.removeToken(pos);
                }
            }
        }));
    }
    
    for(auto& f : futures) f.get();
    
    // Проверяем конечное состояние
    auto tokens = env.getTokens();
    EXPECT_TRUE(tokens.empty());  // Все токены должны быть удалены
}

TEST_F(ComplexScenariosTest, SensorNetworkOverlap) {
    std::vector<std::shared_ptr<Platform>> platforms;
    std::vector<std::shared_ptr<SensorModule>> sensors;
    
    // Создаем перекрывающиеся зоны действия сенсоров
    for(int i = 0; i < 4; i++) {
        auto platform = std::make_shared<StaticPlatform>(
            Pair{20 + (i%2)*5, 20 + (i/2)*5}, 
            &env, "Platform" + std::to_string(i), 100, 3);
        auto sensor = std::make_shared<SensorModule>(1, 10, true, 6, 
            i % 2 == 0 ? SensorType::Optical : SensorType::XRay);
        
        platform->installModule(sensor);
        platforms.push_back(platform);
        sensors.push_back(sensor);
        env.addToken(platform);
    }
    
    // Добавляем подозреваемого в зону перекрытия
    auto suspect = std::make_shared<Suspect>(Pair{22, 22}, &env, 3, 2);
    env.addToken(suspect);
    
    // Параллельно сканируем всеми сенсорами
    std::vector<std::future<Report>> reports;
    for(auto& sensor : sensors) {
        reports.push_back(std::async(std::launch::async, [&]() {
            return sensor->getSurrounding();
        }));
    }
    
    // Все сенсоры должны обнаружить подозреваемого
    for(auto& future : reports) {
        auto report = future.get();
        EXPECT_FALSE(report.objects.empty());
    }
}

TEST_F(ComplexScenariosTest, WeaponInterference) {
    // Создаем несколько платформ с оружием, стреляющих по одной цели
    std::vector<std::shared_ptr<Platform>> platforms;
    std::vector<std::shared_ptr<WeaponModule>> weapons;
    
    for(int i = 0; i < 4; i++) {
        auto platform = std::make_shared<StaticPlatform>(
            Pair{15 + i*2, 15}, &env, "Platform" + std::to_string(i), 100, 3);
        auto weapon = std::make_shared<WeaponModule>(1, 10, true, 5, 
            std::chrono::milliseconds(50));
        
        platform->installModule(weapon);
        platforms.push_back(platform);
        weapons.push_back(weapon);
        env.addToken(platform);
    }
    
    auto suspect = std::make_shared<Suspect>(Pair{20, 15}, &env, 3, 2);
    env.addToken(suspect);
    
    // Параллельная зарядка и стрельба
    std::vector<std::future<void>> futures;
    for(auto& weapon : weapons) {
        futures.push_back(std::async(std::launch::async, [&]() {
            weapon->startCharging();
            std::this_thread::sleep_for(std::chrono::milliseconds(60));
            weapon->attack(suspect->getPosition());
        }));
    }
    
    for(auto& f : futures) f.get();
    
    // Подозреваемый должен быть уничтожен только один раз
    EXPECT_EQ(env.getToken(Pair{20, 15}), nullptr);
} 