#include "mobile_platform.h"

#include "suspect.h"
#include "platform.h"
#include "environment.h"
#include "module_types.h"

Pair MobilePlatform::findPursuitableSuspect(Report report) const {
    for (const auto& token : report.objects)
        if (Suspect* suspect = dynamic_cast<Suspect*>(token.get()))
            if (environment_->hasLineOfSight(position_, suspect->getPosition()) && environment_->howFar(position_, suspect->getPosition(), speed_) <= 1)
                return suspect->getPosition();
    return {-1, 0};
}

void MobilePlatform::positionRelatedUpdate(Pair newPosition) {
    if (newPosition == position_) return;
    ConnectionModule* connectionModule = findModuleOfType<ConnectionModule>();
    std::vector<ConnectionModule*> newNeighborsList = connectionModule->scanForModules(newPosition);
    for (auto module : newNeighborsList)
        if (std::find(connectionModule->getSessionList().begin(), connectionModule->getSessionList().end(), module) == connectionModule->getSessionList().end()) //если это новый сосед
            connectionModule->establishConnection(module, false);
    for (auto module : connectionModule->getSessionList())
        if (std::find(newNeighborsList.begin(), newNeighborsList.end(), module) == newNeighborsList.end()) //если это больше не сосед
            connectionModule->closeConnection(module, false);
}