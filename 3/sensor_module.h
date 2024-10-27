#pragma once

#include "module.h"

enum SensorType {
    Optical,
    XRay
};

struct SurroundingReport {
    Pair position_;
    std::vector<Placeholder*> relatives;
};

class SensorModule : public Module {
private:
    SensorType type_ = SensorType::Optical;

public:
    SensorModule() : Module{} {}
    SensorModule(int slotsOccupied, int energyConsumption, bool isOn, int range, Platform* host, SensorType type)
        : Module(slotsOccupied, energyConsumption, isOn, range, host), type_(type) {}

    SensorType getType() const;
    void setType(SensorType type);

    SurroundingReport getReport();
};