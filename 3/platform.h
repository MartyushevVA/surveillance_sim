#pragma once

#include <string>
#include <vector>
#include <memory>
#include "placeholder.h"

class Module;

class Platform : public Placeholder {
protected:
    std::string description_ {};
    int energyLevel_ = 0;
    int slotCount_ = 0;
    std::vector<std::unique_ptr<Module>> modules_ {};

    Platform() = default;
    Platform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount)
        : Placeholder({x, y}, environment), description_(description), energyLevel_(energyLevel), slotCount_(slotCount) {}

public:
    virtual ~Platform() = default;
    
    void setDescription(std::string description);
    std::string getDescription() const;
    void setEnergyLevel(int energyLevel);
    int getEnergyLevel() const;
    void setSlotCount(int slotCount);
    int getSlotCount() const;

    void installModule(std::unique_ptr<Module> module);
    std::unique_ptr<Module> removeModule(Module* module);
    std::vector<std::unique_ptr<Module>> getModules() const;
};