#include "static_platform.h"

#include "module_types.h"

void StaticPlatform::positionRelatedUpdate(Pair position) {
    if (auto connectionModule = findModuleOfType<ConnectionModule>())
        connectionModule->positionRelatedUpdate(position);
    if (auto sensorModule = findModuleOfType<SensorModule>())
        sensorModule->positionRelatedUpdate(position);
    if (auto weaponModule = findModuleOfType<WeaponModule>())
        weaponModule->positionRelatedUpdate(position);
}