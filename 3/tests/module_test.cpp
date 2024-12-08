#include <gtest/gtest.h>
#include <random>
#include <future>
#include "modules/modules.h"
#include "objects/objects.h"

class ModuleTest : public ::testing::Test {
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
};

TEST_F(ModuleTest, ConcurrentSensorScanning) {
    auto platform = std::make_shared<StaticPlatform>(Pair{25, 25}, &env, "Test Platform", 100, 3);
    auto sensor = std::make_shared<SensorModule>(1, 10, true, 10, SensorType::Optical);
    platform->installModule(sensor);
    env.addToken(platform);
    
    // Создаем несколько подозреваемых
    std::vector<std::shared_ptr<Suspect>> suspects;
    for(int i = 0; i < 5; i++) {
        auto suspect = std::make_shared<Suspect>(getRandomPosition(), &env, 2, 2);
        suspects.push_back(suspect);
        env.addToken(suspect);
    }
    
    // Параллельное сканирование
    std::vector<std::future<Report>> futures;
    for(int i = 0; i < 10; i++) {
        futures.push_back(std::async(std::launch::async, [&]() {
            return sensor->getSurrounding();
        }));
    }
    
    std::vector<Report> reports;
    for(auto& f : futures) {
        reports.push_back(f.get());
    }
    
    // Проверяем консистентность отчетов
    for(const auto& report : reports) {
        EXPECT_EQ(report.objects.size(), suspects.size());
    }
}

TEST_F(ModuleTest, WeaponConcurrentCharging) {
    auto platform = std::make_shared<StaticPlatform>(Pair{25, 25}, &env, "Test Platform", 100, 3);
    auto weapon = std::make_shared<WeaponModule>(1, 10, true, 5, std::chrono::milliseconds(100));
    platform->installModule(weapon);
    
    // Параллельная зарядка и проверка состояния
    std::vector<std::future<void>> futures;
    std::atomic<int> chargeAttempts{0};
    
    for(int i = 0; i < 5; i++) {
        futures.push_back(std::async(std::launch::async, [&]() {
            weapon->startCharging();
            chargeAttempts++;
        }));
    }
    
    for(auto& f : futures) f.get();
    
    EXPECT_EQ(chargeAttempts, 5);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    EXPECT_TRUE(weapon->getIsCharged());
}

TEST_F(ModuleTest, NetworkFormationStress) {
    std::vector<std::shared_ptr<Platform>> platforms;
    std::vector<std::shared_ptr<ConnectionModule>> connections;
    
    // Создаем сеть из 10 платформ
    for(int i = 0; i < 10; i++) {
        auto platform = std::make_shared<MobilePlatform>(
            getRandomPosition(), &env, "Platform" + std::to_string(i), 100, 3, 2);
        auto connection = std::make_shared<ConnectionModule>(1, 10, true, 15, 5);
        platform->installModule(connection);
        platforms.push_back(platform);
        connections.push_back(connection);
        env.addToken(platform);
    }
    
    // Параллельное обновление соединений
    std::vector<std::future<void>> futures;
    for(auto& conn : connections) {
        futures.push_back(std::async(std::launch::async, [&]() {
            for(int i = 0; i < 5; i++) {
                conn->update();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }));
    }
    
    for(auto& f : futures) f.get();
    
    // Проверяем, что сеть сформировалась корректно
    for(auto& conn : connections) {
        EXPECT_FALSE(conn->getSessionList().empty());
    }
}

TEST_F(ModuleTest, DynamicNetworkRestructuring) {
    std::vector<std::shared_ptr<Platform>> platforms;
    
    // Создаем начальную сеть
    for(int i = 0; i < 5; i++) {
        auto platform = std::make_shared<MobilePlatform>(
            Pair{10 + i*5, 10 + i*5}, &env, "Platform" + std::to_string(i), 100, 3, 2);
        auto connection = std::make_shared<ConnectionModule>(1, 10, true, 10, 5);
        platform->installModule(connection);
        platforms.push_back(platform);
        env.addToken(platform);
    }
    
    // Параллельно двигаем платформы и обновляем соединения
    std::vector<std::future<void>> futures;
    for(int i = 0; i < platforms.size(); i++) {
        futures.push_back(std::async(std::launch::async, [&, i]() {
            for(int j = 0; j < 5; j++) {
                platforms[i]->move(getRandomPosition());
                platforms[i]->findModuleOfType<ConnectionModule>()->update();
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }));
    }
    
    for(auto& f : futures) f.get();
    
    // Проверяем, что все платформы все еще имеют соединения
    for(auto& platform : platforms) {
        auto conn = platform->findModuleOfType<ConnectionModule>();
        EXPECT_FALSE(conn->getSessionList().empty());
    }
}

TEST_F(ModuleTest, SensorInterference) {
    // Создаем несколько сенсоров разных типов в одной области
    auto platform1 = std::make_shared<StaticPlatform>(Pair{25, 25}, &env, "Platform1", 100, 3);
    auto platform2 = std::make_shared<StaticPlatform>(Pair{26, 26}, &env, "Platform2", 100, 3);
    
    auto opticalSensor = std::make_shared<SensorModule>(1, 10, true, 10, SensorType::Optical);
    auto xraySensor = std::make_shared<SensorModule>(1, 10, true, 10, SensorType::XRay);
    
    platform1->installModule(opticalSensor);
    platform2->installModule(xraySensor);
    
    env.addToken(platform1);
    env.addToken(platform2);
    
    // Добавляем препятствие и подозреваемого
    auto obstacle = std::make_shared<Obstacle>(Pair{27, 27}, &env);
    auto suspect = std::make_shared<Suspect>(Pair{28, 28}, &env, 2, 2);
    
    env.addToken(obstacle);
    env.addToken(suspect);
    
    // Параллельное сканирование разными сенсорами
    auto futureOptical = std::async(std::launch::async, [&]() {
        return opticalSensor->getSurrounding();
    });
    
    auto futureXRay = std::async(std::launch::async, [&]() {
        return xraySensor->getSurrounding();
    });
    
    auto opticalReport = futureOptical.get();
    auto xrayReport = futureXRay.get();
    
    // X-Ray должен видеть сквозь препятствие
    EXPECT_LT(opticalReport.objects.size(), xrayReport.objects.size());
}