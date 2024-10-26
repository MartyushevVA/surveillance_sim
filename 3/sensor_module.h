#pragma once

#include "module.h"

enum SensorType {
    Optical,
    XRay
};

struct SurroundingReport {
    Pair position_;
    std::vector<Placeholder> relatives;
};

class SensorModule : public Module {
private:
    SensorType type_;
    int energyConsumption_;

public:
    SensorModule(SensorType type = SensorType::Optical, int energyConsumption = 0) : Module{}, type_(type), energyConsumption_(energyConsumption) {}
    SensorModule(int x, int y, int slotsOccupied, bool isOn, int range, SensorType type = SensorType::Optical, int energyConsumption = 0)
        : Module(x, y, slotsOccupied, isOn, range), type_(type), energyConsumption_(energyConsumption) {}

    SurroundingReport getReport();
    void activate() override;
    void deactivate() override;
};