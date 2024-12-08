#include <gtest/gtest.h>
#include <random>
#include <thread>
#include <future>
#include "objects/objects.h"

class MovementTest : public ::testing::Test {
protected:
    Environment env;
    std::mt19937 gen{std::random_device{}()};
    
    void SetUp() override {
        env.setSize(50, 50);  // Больше поле для случайных тестов
    }
    
    Pair getRandomPosition() {
        std::uniform_int_distribution<> dis(0, 49);
        return {dis(gen), dis(gen)};
    }
    
    int getRandomSpeed() {
        std::uniform_int_distribution<> dis(1, 5);
        return dis(gen);
    }
};

TEST_F(MovementTest, SuspectEscapesBehavior) {
    auto suspect = std::make_shared<Suspect>(Pair{25, 25}, &env, 3, getRandomSpeed());
    auto platform = std::make_shared<MobilePlatform>(Pair{20, 20}, &env, "Hunter", 100, 3, getRandomSpeed());
    env.addToken(suspect);
    env.addToken(platform);

    // Запоминаем начальную дистанцию
    double initialDistance = env.calculateDistance(suspect->getPosition(), platform->getPosition());
    
    // Делаем несколько ходов
    for(int i = 0; i < 10; i++) {
        Pair newPos = suspect->opponentBasedMove(platform->getPosition());
        suspect->move(newPos);
        double newDistance = env.calculateDistance(suspect->getPosition(), platform->getPosition());
        EXPECT_GE(newDistance, initialDistance) << "Suspect should try to maintain or increase distance";
    }
}

TEST_F(MovementTest, SuspectEscapeFromMultiplePlatforms) {
    auto suspect = std::make_shared<Suspect>(Pair{25, 25}, &env, 3, getRandomSpeed());
    env.addToken(suspect);
    
    std::vector<std::shared_ptr<MobilePlatform>> platforms;
    for(int i = 0; i < 3; i++) {
        auto platform = std::make_shared<MobilePlatform>(
            getRandomPosition(), &env, "Hunter" + std::to_string(i), 100, 3, getRandomSpeed());
        platforms.push_back(platform);
        env.addToken(platform);
    }

    Pair initialPos = suspect->getPosition();
    suspect->move(suspect->opponentBasedMove(platforms[0]->getPosition()));
    
    // Проверяем, что подозреваемый не остался на месте
    EXPECT_NE(suspect->getPosition(), initialPos);
}

TEST_F(MovementTest, ConcurrentMovement) {
    std::vector<std::shared_ptr<Suspect>> suspects;
    std::vector<std::shared_ptr<MobilePlatform>> platforms;
    
    // Создаем несколько подозреваемых и платформ
    for(int i = 0; i < 5; i++) {
        suspects.push_back(std::make_shared<Suspect>(
            getRandomPosition(), &env, 3, getRandomSpeed()));
        platforms.push_back(std::make_shared<MobilePlatform>(
            getRandomPosition(), &env, "Platform" + std::to_string(i), 100, 3, getRandomSpeed()));
    }
    
    // Добавляем их в среду
    for(auto& s : suspects) env.addToken(s);
    for(auto& p : platforms) env.addToken(p);
    
    // Запускаем параллельное движение
    std::vector<std::future<void>> futures;
    for(int i = 0; i < 5; i++) {
        futures.push_back(std::async(std::launch::async, [&, i]() {
            for(int j = 0; j < 10; j++) {
                Pair newPos = suspects[i]->opponentBasedMove(platforms[i]->getPosition());
                suspects[i]->move(newPos);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }));
    }
    
    // Ждем завершения всех движений
    for(auto& f : futures) f.get();
    
    // Проверяем, что все подозреваемые все еще существуют и находятся в валидных позициях
    for(auto& s : suspects) {
        ASSERT_NE(s, nullptr);
        auto pos = s->getPosition();
        EXPECT_GE(pos.x, 0);
        EXPECT_GE(pos.y, 0);
        EXPECT_LT(pos.x, 50);
        EXPECT_LT(pos.y, 50);
    }
}