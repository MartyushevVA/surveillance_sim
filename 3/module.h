#pragma once

#include <vector>
#include "environment.h"

class Module {
protected:
    Pair hostPosition_ = {0, 0};
    int slotsOccupied_ = 0;
    bool isOn_ = false;
    int range_ = 0;
    Module() = default;
    Module(int x, int y, int slotsOccupied, bool isOn, int range) :
        hostPosition_({x, y}), slotsOccupied_(slotsOccupied), isOn_(isOn), range_(range) {}

public:
    virtual ~Module() = default;

    void setEnergyLevel(int energyLevel);
    int getEnergyLevel() const;
    void setSlotCount(int slotCount);
    int getSlotCount() const;
    void setState(bool state);
    bool getState() const;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
};