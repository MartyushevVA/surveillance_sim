#pragma once

#include "platform.h"

class MobilePlatform : public Platform {
private:
    int speed_;

public:
    MobilePlatform() = default;
    MobilePlatform(int x, int y, const std::string& description, int energyLevel, int slotCount, int speed);
    ~MobilePlatform() = default;
    
    void setSpeed(int speed);
    int getSpeed() const;

    void move(int dx, int dy) override;
};