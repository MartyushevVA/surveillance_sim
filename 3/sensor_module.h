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

    SensorType getType() const;
    void setType(SensorType type);

    SurroundingReport getReport() const override;
};