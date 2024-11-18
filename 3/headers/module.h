#pragma once

#include <memory>
#include <stdexcept>

#include "platform.h"

class Module {
protected:
    int slotsOccupied_ = 0;
    int energyConsumption_ = 0;
    bool isOn_ = false;
    int range_ = 0;
    Platform* host_ = nullptr;

    Module() = default;
    Module(int slotsOccupied, int energyConsumption, bool isOn, int range)
        : slotsOccupied_(slotsOccupied), energyConsumption_(energyConsumption), isOn_(isOn), range_(range) {}

public:
    virtual ~Module() = default;

    int getSlotsOccupied() const {return slotsOccupied_;}
    void setSlotsOccupied(int slotsOccupied) {if (slotsOccupied > 0) slotsOccupied_ = slotsOccupied;}
    int getEnergyConsumption() const {return energyConsumption_;}
    void setEnergyConsumption(int energyConsumption) {if (energyConsumption > 0) energyConsumption_ = energyConsumption;}
    bool getIsOn() const {return isOn_;}
    void setIsOn(bool isOn) {isOn_ = isOn;}
    int getRange() const {return range_;}
    void setRange(int range) {if (range > 0) range_ = range;}
    Platform* getHost() const {return host_;}
    void setHost(Platform* host) {host_ = host;}

    bool isInRange(Pair target) const;
    virtual void attachTo(Platform* host) const = 0;
};