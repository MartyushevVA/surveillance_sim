#pragma once

#include <memory>

#include "../objects/base_platform.h"
#include "../system/environment.h"

class Module {
protected:
    int slotsOccupied_ = 0;
    int energyConsumption_ = 0;
    bool isOn_ = false;
    int range_ = 0;
    std::weak_ptr<Platform> host_ {};

    Module(int slotsOccupied, int energyConsumption, bool isOn, int range)
        : slotsOccupied_(slotsOccupied), energyConsumption_(energyConsumption), isOn_(isOn), range_(range) {}

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
    void setHost(std::weak_ptr<Platform> host) {
        host_ = host;
        setUp();
    }
    bool isInRange(Pair target) const {
        auto hostPtr = host_.lock();
        if (hostPtr)
            return hostPtr->getEnvironment()->howFar(target, hostPtr->getPosition(), range_) <= range_;
        return false;
    }
    virtual bool attachableTo(std::shared_ptr<Platform> host) const = 0;
    virtual void update() = 0;
    virtual void setUp() = 0;
};