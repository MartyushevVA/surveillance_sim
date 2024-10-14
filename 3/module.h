#pragma once

class Module {
protected:
    int energyLevel_;
    int slotCount_;
    bool state_;

    int range_;
public:
    Module() = default;
    virtual ~Module() = default;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
};