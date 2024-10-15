#pragma once

#include <string>
#include <vector>
#include "module.h"

class Platform {
protected:
    int x_;
    int y_;
    std::string description_;
    int energyLevel_;
    int slotCount_;
    std::vector<Module&> modules_;

public:
    Platform() = default;
    virtual ~Platform() = default;
    
    void setPosition(int x, int y);
    std::pair<int, int> getPosition() const;
    void setEnergyLevel(int energyLevel);
    int getEnergyLevel() const;
    void setSlotCount(int slotCount);
    int getSlotCount() const;

    void addModule(Module& module);
    void removeModule(Module& module);
    virtual void move(int dx, int dy) = 0;
};