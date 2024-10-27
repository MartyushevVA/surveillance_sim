#pragma once

#include <vector>
#include <memory>
#include "placeholder.h"
#include "module.h"

enum SensorType {
    Optical,
    XRay
};

struct SurroundingReport {
    Pair position_;
    std::vector<std::shared_ptr<Placeholder>> relatives;
};

class SensorModule : public Module {
private:
    SensorType type_ = SensorType::Optical;

public:
    SensorModule() : Module{} {}
    SensorModule(int slotsOccupied, int energyConsumption, bool isOn, int range, SensorType type)
        : Module(slotsOccupied, energyConsumption, isOn, range), type_(type) {}

    SensorType getType() const;
    void setType(SensorType type);

    SurroundingReport getReport();
};