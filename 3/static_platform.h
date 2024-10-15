#pragma once

#include "platform.h"

class StaticPlatform : public Platform {
public:
    using Platform::Platform;
    StaticPlatform(int x, int y, const std::string& description, int energyLevel, int slotCount);
    ~StaticPlatform() = default;
    
    void move(int dx, int dy) override;
};