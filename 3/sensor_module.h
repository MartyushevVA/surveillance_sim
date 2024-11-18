#pragma once

#include "module.h"
#include "interfaces_fromModules.h"

enum SensorType {
    Optical,
    XRay
};

class SensorModule :
    public Module,
    public ISensor {
protected:
    SensorType type_ = SensorType::Optical;

public:
    SensorModule() = default;
    SensorModule(int slotsOccupied, int energyConsumption, bool isOn, int range, SensorType type)
        : Module(slotsOccupied, energyConsumption, isOn, range), type_(type) {}

    SensorType getType() const {return type_;}
    void setType(SensorType type) {type_ = type;}

    std::vector<std::shared_ptr<Placeholder>> getSurrounding() const override;
    void attachTo(Platform* host) const override;
};