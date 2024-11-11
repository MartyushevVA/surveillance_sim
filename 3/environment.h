#pragma once

#include <set>
#include <memory>
#include "placeholder.h"

enum class CellType {
    Empty,
    Obstacle,
    Platform,
    Intruder
};

class Obstacle : public Placeholder {};

class Environment {
private:
    Pair size_ = {0, 0};
    std::set<std::shared_ptr<Placeholder>> tokens_ {};

public:
    Environment() = default;
    Environment(int width, int height) : size_({width, height}) {}

    Pair getSize() const {return size_;}
    void setSize(int width, int height) {
        if (width > 0 && height > 0)
            size_ = {width, height};
    }
    
    void addToken(std::shared_ptr<Placeholder> token);
    std::shared_ptr<Placeholder> removeToken(int x, int y);

    std::set<std::shared_ptr<Placeholder>> getTokens() const {return tokens_;}
    
    CellType getCellType(int x, int y) const;
    void setCellType(int x, int y, CellType type);
};

//setcelltype теперь должен принимать какую именно платформу нужно создать, тк иначе у нее нет координат
//и это логично тк базовый класс платформы не может иметь объекта.