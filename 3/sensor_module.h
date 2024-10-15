#pragma once

#include "module.h"
#include <vector>
#include "platform.h"
#include "environment.h"
#include "intruder.h"


enum mod{
    Optical,
    XRay
};

struct Surrounding {
    std::vector<Platform&> platforms;
    std::vector<Intruder&> intruders;
    int x;
    int y;
};

class SensorModule : public Module {
private:
    mod type_;
public:
    SensorModule() = default;
    explicit SensorModule(mod type);
    ~SensorModule() = default;
    
    Surrounding getSurroundings();
    void activate() override;
    void deactivate() override;
};