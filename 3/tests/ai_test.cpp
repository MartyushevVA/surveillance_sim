#include <gtest/gtest.h>
#include <random>
#include <future>
#include "system/ai.h"
#include "objects/objects.h"
#include "modules/modules.h"

class AITest : public ::testing::Test {
protected:
    Environment env;
    AI ai;
    std::mt19937 gen{std::random_device{}()};
    
    AITest() : env(), ai(&env) {
        env.setSize(50, 50);
    }
    
    Pair getRandomPosition() {
        std::uniform_int_distribution<> dis(0, 49);
        return {dis(gen), dis(gen)};
    }
    
    std::shared_ptr<Platform> createArmedPlatform(Pair pos, bool mobile = false) {
        std::shared_ptr<Platform> platform;
        if (mobile) {
            platform = std::make_shared<MobilePlatform>(pos, &env, "Armed Platform", 100, 3, 2);
        } else {
            platform = std::make_shared<StaticPlatform>(pos, &env, "Armed Platform", 100, 3);
        }
        
        auto sensor = std::make_shared<SensorModule>(1, 10, true, 8, SensorType::Optical);
        auto weapon = std::make_shared<WeaponModule>(1, 10, true, 5, std::chrono::milliseconds(50));
        auto connection = std::make_shared<ConnectionModule>(1, 10, true, 10, 5);
        
        platform->installModule(sensor);
        platform->installModule(weapon);
        platform->installModule(connection);
        
        return platform;
    }
};

TEST_F(AITest, ConcurrentSuspectElimination) {
    // Создаем сеть из платформ
    std::vector<std::shared_ptr<Platform>> platforms;
    for(int i = 0; i < 3; i++) {
        auto platform = createArmedPlatform(getRandomPosition(), true);
        platforms.push_back(platform);
        env.addToken(platform);
        ai.addStaticPlatform(platform.get());
    }
    
    // Создаем несколько подозреваемых
    std::vector<std::shared_ptr<Suspect>> suspects;
    for(int i = 0; i < 5; i++) {
        auto suspect = std::make_shared<Suspect>(getRandomPosition(), &env, 3, 2);
        suspects.push_back(suspect);
        env.addToken(suspect);
    }
    
    // Запускаем параллельное уничтожение
    std::vector<std::future<void>> futures;
    for(int i = 0; i < 3; i++) {
        futures.push_back(std::async(std::launch::async, [&]() {
            ai.eliminateAllSuspects();
        }));
    }
    
    for(auto& f : futures) f.get();
    
    // Проверяем, что все подозреваемые были уничтожены
    for(const auto& suspect : suspects) {
        EXPECT_EQ(env.getToken(suspect->getPosition()), nullptr);
    }
}

TEST_F(AITest, NetworkResilience) {
    // Создаем сеть платформ с перекрывающимися зонами связи
    auto platform1 = createArmedPlatform(Pair{10, 10});
    auto platform2 = createArmedPlatform(Pair{15, 15});
    auto platform3 = createArmedPlatform(Pair{20, 20});
    
    env.addToken(platform1);
    env.addToken(platform2);
    env.addToken(platform3);
    
    ai.addStaticPlatform(platform1.get());
    ai.addStaticPlatform(platform2.get());
    ai.addStaticPlatform(platform3.get());
    
    // Добавляем подозреваемого
    auto suspect = std::make_shared<Suspect>(Pair{12, 12}, &env, 3, 2);
    env.addToken(suspect);
    
    // Удаляем среднюю платформу во время работы
    std::thread elimination([&]() {
        ai.eliminateAllSuspects();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    env.removeToken(platform2->getPosition());
    
    elimination.join();
    
    // Проверяем, что подозреваемый все равно был уничтожен
    EXPECT_EQ(env.getToken(suspect->getPosition()), nullptr);
}

TEST_F(AITest, SimultaneousTargeting) {
    // Создаем две платформы, нацеленные на одного подозреваемого
    auto platform1 = createArmedPlatform(Pair{10, 10});
    auto platform2 = createArmedPlatform(Pair{14, 14});
    auto suspect = std::make_shared<Suspect>(Pair{12, 12}, &env, 3, 2);
    
    env.addToken(platform1);
    env.addToken(platform2);
    env.addToken(suspect);
    
    ai.addStaticPlatform(platform1.get());
    ai.addStaticPlatform(platform2.get());
    
    // Запускаем атаку с обеих платформ
    std::future<void> attack1 = std::async(std::launch::async, [&]() {
        platform1->findModuleOfType<WeaponModule>()->attack(suspect->getPosition());
    });
    
    std::future<void> attack2 = std::async(std::launch::async, [&]() {
        platform2->findModuleOfType<WeaponModule>()->attack(suspect->getPosition());
    });
    
    attack1.get();
    attack2.get();
    
    // Проверяем, что не возникло ошибок при одновременной атаке
    EXPECT_EQ(env.getToken(suspect->getPosition()), nullptr);
}

// Продолжение следует... 