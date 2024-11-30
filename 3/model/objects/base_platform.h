#pragma once

#include "placeholder.h"

class Module;

class Platform : 
    public Placeholder,
    public std::enable_shared_from_this<Platform> {
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
    
    std::string getDescription() const {return description_;}
    int getEnergyLevel() const {return energyLevel_;}
    void setEnergyLevel(int energyLevel) {
        if (energyLevel < 0)
            throw std::invalid_argument("Energy level cannot be negative");
        energyLevel_ = energyLevel;
    }
    int getMaxEnergyLevel() const {return maxEnergyLevel_;}
    int getSlotCount() const {return slotCount_;}
    
    std::vector<std::shared_ptr<Module>> getModules() const {return modules_;}

    void installModule(std::shared_ptr<Module> module);

    void update() override;

    template<typename T>
    T* findModuleOfType() const {
        for (const auto& module : getModules())
            if (T* typed_module = dynamic_cast<T*>(module.get()))
                return typed_module;
        return nullptr;
    }
};