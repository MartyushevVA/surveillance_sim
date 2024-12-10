#pragma once

#include <vector>
#include <map>
#include <memory>

#include "../common_types.h"
#include "../objects/objects.h"
#include "environment.h"

class Environment;
class Platform;
class StaticPlatform;
class MobilePlatform;
class Suspect;
class Placeholder;

class AI : public std::enable_shared_from_this<AI> {
private:
    std::weak_ptr<Environment> environment_;
    std::vector<std::shared_ptr<ConnectionModule>> mainHosts_ {};
    std::vector<std::shared_ptr<ConnectionModule>> allConnectedPlatforms_ {};
    std::map<Pair, std::shared_ptr<Suspect>> spottedSuspects_ {};  

public:
    AI(std::shared_ptr<Environment> environment);

    void addMainHost(std::shared_ptr<ConnectionModule> host);
    void eliminateAllSuspects();
    void getNetworkForest();
    const std::vector<std::shared_ptr<ConnectionModule>>& getAllConnectedPlatforms() const {return allConnectedPlatforms_;}
    
    void addSuspects(std::map<Pair, std::shared_ptr<Suspect>> suspects);
    void removeSuspect(std::shared_ptr<Suspect> suspect);
    const std::map<Pair, std::shared_ptr<Suspect>>& getSuspects() const {return spottedSuspects_;}

    std::shared_ptr<AI> getSharedPtr() { return shared_from_this(); }
};