#pragma once

#include <memory>

#include "../objects/base_platform.h"
#include "../system/environment.h"

class Module {
protected:
    int slotsOccupied_;
    int energyConsumption_;
    int range_;
    bool isOn_;
    std::weak_ptr<Platform> host_ {};

    Module(int slotsOccupied, int energyConsumption, int range)
        : slotsOccupied_(slotsOccupied), energyConsumption_(energyConsumption), range_(range), isOn_(false) {}

public:
    virtual ~Module() = default;

    int getSlotsOccupied() const {return slotsOccupied_;}
    void setSlotsOccupied(int slotsOccupied) {slotsOccupied_ = slotsOccupied;}
    int getEnergyConsumption() const {return energyConsumption_;}
    void setEnergyConsumption(int energyConsumption) {energyConsumption_ = energyConsumption;}
    bool getIsOn() const {return isOn_;}
    void setIsOn(bool isOn) {isOn_ = isOn;}
    int getRange() const {return range_;}
    void setRange(int range) {range_ = range;}
    std::shared_ptr<Platform> getHost() const {return host_.lock();}
    void setHost(std::weak_ptr<Platform> host) {host_ = host;}

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    
    virtual void update() = 0;
};