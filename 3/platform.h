#pragma once

#include <string>
#include <vector>
#include <memory>
#include "placeholder.h"

class Module;
class Environment;

class Platform : public Placeholder {
protected:
    std::string description_ = "";
    int energyLevel_ = 0;
    int slotCount_ = 0;
    std::vector<std::shared_ptr<Module>> modules_;

    Platform() : Placeholder{} {}
    Platform(int x, int y, Environment* environment, std::string description, int energyLevel, int slotCount)
        : Placeholder({x, y}, environment), description_(description), energyLevel_(energyLevel), slotCount_(slotCount) {}

public:
    virtual ~Platform() = default;
    
    void setEnergyLevel(int energyLevel);
    int getEnergyLevel() const;
    void setSlotCount(int slotCount);
    int getSlotCount() const;

    void installModule(std::shared_ptr<Module> module);
    void removeModule(std::shared_ptr<Module> module);
    const std::vector<std::shared_ptr<Module>>& getModules() const;
    void setModules(std::vector<std::shared_ptr<Module>> modules);
};