#pragma once

#include <string>
#include <vector>
#include <memory>
#include "placeholder.h"

class Module;

class Platform : virtual public Placeholder {
protected:
    std::string description_ {};
    int energyLevel_ = 0;
    int maxEnergyLevel_ = 0;
    int slotCount_ = 0;
    std::vector<std::unique_ptr<Module>> modules_ {};

    Platform() = default;
    Platform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount)
        : Placeholder({x, y}, environment), description_(description), energyLevel_(energyLevel), slotCount_(slotCount) {}

public:
    virtual ~Platform() = default;
    
    void setDescription(std::string description) {description_ = std::move(description);}
    std::string getDescription() const {return description_;}
    void setEnergyLevel(int energyLevel) {
        if (energyLevel > 0)
            energyLevel_ = energyLevel;
    }
    int getEnergyLevel() const {return energyLevel_;}
    void setSlotCount(int slotCount) {
        if (slotCount > 0)
            slotCount_ = slotCount;
    }
    int getSlotCount() const {return slotCount_;}
    std::vector<std::unique_ptr<Module>> getModules() const {return modules_;}

    void installModule(std::unique_ptr<Module> module);
    std::unique_ptr<Module> removeModule(std::unique_ptr<Module> module);
    
};