#include <iostream>
#include <chrono>
#include <vector>
#include <functional>
#include <memory>
#include <cmath>
#include <future>
#include "system/environment.h"
#include "objects/objects.h"

class Field : public Environment {
public:
    Field(Pair size) : Environment(size) {}
};

class ConcretePlaceholder : public Suspect {
public:
    ConcretePlaceholder(Pair position, std::weak_ptr<Environment> env) : Suspect(position, env, 3, 3) {}
};

void testTokenOperations(int numTokens) {
    auto env = std::make_shared<Field>(Pair{(int)sqrt(numTokens) + 1, (int)sqrt(numTokens) + 1});

    std::vector<std::shared_ptr<ConcretePlaceholder>> tokens;
    for (int i = 0; i < numTokens; ++i) {
        tokens.push_back(std::make_shared<ConcretePlaceholder>(Pair{i % ((int)sqrt(numTokens) + 1), i / ((int)sqrt(numTokens) + 1)}, env));
    }

    for (const auto& token : tokens)
        env->addToken(token);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::future<void>> futures;
    for (auto& token : tokens) {
        futures.push_back(std::async(std::launch::async, [&token, &env]() {
            Pair newPos = { (token->getPosition().x + 1) % 100, (token->getPosition().y + 1) % 100 };
            token->move(newPos);
        }));
    }
    for (auto& future : futures) {
        future.get();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> moveTime = end - start;
    std::cout << "Time to move " << numTokens << " tokens: " << moveTime.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    futures.clear();
    for (const auto& token : tokens) {
        futures.push_back(std::async(std::launch::async, [&token, &env]() {
            env->removeToken(token->getPosition());
        }));
    }
    for (auto& future : futures) {
        future.get();
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> removeTime = end - start;
    std::cout << "Time to remove " << numTokens << " tokens: " << removeTime.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    futures.clear();
    for (const auto& token : tokens) {
        futures.push_back(std::async(std::launch::async, [&token, &env]() {
            env->getArea(token->getPosition(), 10);
        }));
    }
    for (auto& future : futures) {
        future.get();
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> getAreaTime = end - start;
    std::cout << "Time to get area: " << getAreaTime.count() << " ms" << std::endl;
}

int main() {
    std::vector<int> tokenCounts = {10000, 20000, 30000, 40000, 50000, 60000};
    for (int count : tokenCounts) {
        testTokenOperations(count);
    }
    return 0;
}