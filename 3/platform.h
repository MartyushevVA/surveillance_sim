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
    Platform(int x, int y, const std::string& description, int energyLevel, int slotCount);
    virtual ~Platform() = default;
    
    void addModule(Module& module);
    void removeModule(Module& module);
    virtual void move(int dx, int dy) = 0;
};