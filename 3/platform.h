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
    int maxEnergyLevel_ = 0;
    int slotCount_ = 0;
    std::vector<std::unique_ptr<Module>> modules_ {};

    Platform() = default;
    Platform(Pair position, Environment* environment, std::string description, int energyLevel, int slotCount, int speed)
        : Placeholder(position, environment, speed), description_(description), energyLevel_(energyLevel), slotCount_(slotCount) {}

public:
    virtual ~Platform() = default;
    
    void setDescription(std::string description) {description_ = std::move(description);}
    std::string getDescription() const {return description_;}
    int getEnergyLevel() const {return energyLevel_;}
    void setEnergyLevel(int energyLevel) {if (energyLevel > 0) energyLevel_ = energyLevel;}
    int getSlotCount() const {return slotCount_;}
    void setSlotCount(int slotCount) {if (slotCount > 0) slotCount_ = slotCount;}
    int getMaxEnergyLevel() const {return maxEnergyLevel_;}
    void setMaxEnergyLevel(int maxEnergyLevel) {if (maxEnergyLevel > 0) maxEnergyLevel_ = maxEnergyLevel;}

    std::vector<std::unique_ptr<Module>> getModules() const {return modules_;}

    void installModule(std::unique_ptr<Module> module);
    std::unique_ptr<Module> removeModule(std::unique_ptr<Module> module);

    template<typename T>
    T* findModuleOfType() {
        for (const auto& module : getModules()) {
            if (T* typed_module = dynamic_cast<T*>(module.get()))
                return typed_module;
        }
        return nullptr;
    }
};