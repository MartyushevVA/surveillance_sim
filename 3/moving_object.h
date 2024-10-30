#pragma once

class MovingObject {
protected:
    int speed_ = 0;
    MovingObject() = default;
    MovingObject(int speed) : speed_(speed) {}
public:
    virtual ~MovingObject() = default;

    void setSpeed(int speed);
    int getSpeed() const;
};