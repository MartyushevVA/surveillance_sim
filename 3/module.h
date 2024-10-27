#pragma once
#include <memory>

class Platform;

class Module {
protected:
    int slotsOccupied_ = 0;
    int energyConsumption_ = 0;
    bool isOn_ = false;
    int range_ = 0;
    std::shared_ptr<Platform> host_;
    
    Module() = default;
    Module(int slotsOccupied, int energyConsumption, bool isOn, int range)
        : slotsOccupied_(slotsOccupied), energyConsumption_(energyConsumption), isOn_(isOn), range_(range) {}

public:
    virtual ~Module() = default;

    int getSlotsOccupied() const;
    void setSlotsOccupied(int slotsOccupied);
    int getEnergyConsumption() const;
    void setEnergyConsumption(int energyConsumption);
    bool getIsOn() const;
    void setIsOn(bool isOn);
    int getRange() const;
    void setRange(int range);
    std::shared_ptr<Platform> getHost() const;
    void setHost(std::shared_ptr<Platform> host);
};