#pragma once

#include <string>
#include <vector>
#include "placeholder.h"
#include "module.h"

class Platform : public Placeholder {
protected:
    std::string description_ = "";
    int energyLevel_ = 0;
    int slotCount_ = 0;
    std::vector<Module*> modules_ = {};

    Platform() : Placeholder{} {}
    Platform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount, std::vector<Module*> modules)
        : Placeholder({x, y}, environment), description_(description), energyLevel_(energyLevel), slotCount_(slotCount), modules_(modules) {}

public:
    virtual ~Platform() = 0;
    
    void setPosition(int x, int y);
    Pair getPosition() const;
    void setEnergyLevel(int energyLevel);
    int getEnergyLevel() const;
    void setSlotCount(int slotCount);
    int getSlotCount() const;

    void installModule(Module* module);
    void removeModule(Module* module);
};