#pragma once

#include <string>
#include <vector>
#include "placeholder.h"
#include "module.h"

class Platform : public Placeholder {
protected:
    Pair position_ = {0, 0};
    std::string description_ = "";
    int energyLevel_ = 0;
    int slotCount_ = 0;
    std::vector<Module*> modules_ = {};

    Platform() = default;
    Platform(int x, int y, std::string description, int energyLevel, int slotCount, std::vector<Module*> modules = {})
        : Placeholder({x, y}), description_(description), energyLevel_(energyLevel), slotCount_(slotCount), modules_(modules) {}

public:
    virtual ~Platform() = default;
    
    void setPosition(int x, int y);
    Pair getPosition() const;
    void setEnergyLevel(int energyLevel);
    int getEnergyLevel() const;
    void setSlotCount(int slotCount);
    int getSlotCount() const;

    void installModule(Module* module);
    void removeModule(Module* module);
};