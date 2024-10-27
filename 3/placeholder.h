#pragma once

class Environment;

struct Pair {
    int x, y;
};

class Placeholder {
protected:
    Pair position_ = {0, 0};
    Environment* environment_ = nullptr;
    Placeholder() = default;
    Placeholder(Pair position, Environment* environment) : position_(position), environment_(environment) {}

public:
    void setPosition(int x, int y);
    Pair getPosition() const;   
};