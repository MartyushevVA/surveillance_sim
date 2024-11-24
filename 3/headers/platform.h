#pragma once

#include <string>

#include "placeholder.h"

class Platform : public Placeholder {
protected:
    std::string description_ {};
    int energyLevel_ = 0;
    int maxEnergyLevel_ = 0;
    int slotCount_ = 0;
    std::vector<std::shared_ptr<Module>> modules_ {};

    Platform(Pair position, Environment* environment, std::string description, int maxEnergyLevel, int slotCount, int speed)
        : Placeholder(position, environment, speed), description_(description), maxEnergyLevel_(maxEnergyLevel), slotCount_(slotCount) {}

public:
    virtual ~Platform() = default;
    
    void setDescription(std::string description) {description_ = std::move(description);}
    std::string getDescription() const {return description_;}
    int getEnergyLevel() const {return energyLevel_;}
    void setEnergyLevel(int energyLevel) {
        if (energyLevel <= 0)
            throw std::invalid_argument("Energy level must be positive");
        energyLevel_ = energyLevel;
    }
    int getSlotCount() const {return slotCount_;}
    void setSlotCount(int slotCount) {
        if (slotCount <= 0)
            throw std::invalid_argument("Slot count must be positive");
        slotCount_ = slotCount;
    }
    int getMaxEnergyLevel() const {return maxEnergyLevel_;}
    void setMaxEnergyLevel(int maxEnergyLevel) {
        if (maxEnergyLevel <= 0)
            throw std::invalid_argument("Max energy level must be positive");
        maxEnergyLevel_ = maxEnergyLevel;
    }
    
    std::vector<std::shared_ptr<Module>> getModules() const {return modules_;}

    void installModule(std::shared_ptr<Module> module);
    std::shared_ptr<Module> extractModule(Module* module);

    template<typename T>
    T* findModuleOfType() {
        for (const auto& module : getModules())
            if (T* typed_module = dynamic_cast<T*>(module.get()))
                return typed_module;
        return nullptr;
    }
};