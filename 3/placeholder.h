#pragma once

class Environment;

struct Pair {
    int x, y;
    bool operator<(const Pair& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

class Placeholder {
protected:
    Pair position_ = {0, 0};
    Environment* environment_ = nullptr;
    Placeholder() = default;
    Placeholder(Pair position, Environment* environment) : position_(position), environment_(environment) {}

public:
    virtual ~Placeholder() = default;

    void setPosition(int x, int y);
    Pair getPosition() const;

    bool operator<(const Placeholder& other) const {
        return position_ < other.position_;
    }
};