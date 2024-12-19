#include <iostream>
#include <chrono>
#include <vector>
#include <functional>
#include <memory>
#include <cmath>
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

    for (const auto& token : tokens) {
        env->addToken(token);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (auto& token : tokens) {
        Pair newPos = { (token->getPosition().x + 1) % 100, (token->getPosition().y + 1) % 100 };
        token->move(newPos);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> moveTime = end - start;
    std::cout << "Time to move " << numTokens << " tokens: " << moveTime.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (const auto& token : tokens) {
        env->removeToken(token->getPosition());
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> removeTime = end - start;
    std::cout << "Time to remove " << numTokens << " tokens: " << removeTime.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (const auto& token : tokens) {
        env->getArea(token->getPosition(), 10);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> getAreaTime = end - start;
    std::cout << "Time to get area: " << getAreaTime.count() << " ms" << std::endl;
}

int main() {
    std::vector<int> tokenCounts = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};
    for (int count : tokenCounts) {
        testTokenOperations(count);
    }
    return 0;
}