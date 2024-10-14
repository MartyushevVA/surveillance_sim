#pragma once

#include "platform.h"

class MobilePlatform : public Platform {
private:
    int speed_;

public:
    MobilePlatform(int x, int y, const std::string& description, int energyLevel, int slotCount, int speed);
    virtual ~MobilePlatform() = default;
    
    void move(int dx, int dy) override;
};