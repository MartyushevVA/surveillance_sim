#pragma once

class Module {
protected:
    int energyLevel_;
    int slotCount_;
    bool state_;

    int range_;
public:
    Module() = default;
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