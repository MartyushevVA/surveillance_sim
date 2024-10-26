#pragma once

struct Pair {
    int x, y;
};

class Placeholder {
protected:
    Pair position_ = {0, 0};
    Placeholder() = default;
    Placeholder(Pair position) : position_(position) {}
};