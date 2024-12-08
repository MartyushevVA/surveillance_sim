#pragma once

#include "../system/environment.h"

#include <iostream>

class Module;

/**
 * @class Platform
 * @brief Class representing a platform that can hold modules and interact with the environment.
 */
class Platform : public std::enable_shared_from_this<Platform> {
protected:
    std::string description_ {};
    int energyLevel_ = 0;
    int maxEnergyLevel_ = 0;
    size_t slotCount_ = 0;
    std::vector<std::shared_ptr<Module>> modules_ {};
    /**
     * @brief Constructs a Platform object.
     * @param position The position of the platform in the environment.
     * @param environment Pointer to the environment the platform belongs to.
     * @param description Description of the platform.
     * @param maxEnergyLevel Maximum energy level of the platform.
     * @param slotCount Number of slots available for modules.
     * @param speed Speed of the platform.
     */
    Platform(std::string description, int maxEnergyLevel, size_t slotCount)
        : description_(description), maxEnergyLevel_(maxEnergyLevel), slotCount_(slotCount) {}

public:
    virtual ~Platform() = default; ///< Destructor for the Platform class.

    /**
     * @brief Gets the description of the platform.
     * @return std::string The description of the platform.
     */
    std::string getDescription() const { return description_; }

    /**
     * @brief Gets the current energy level of the platform.
     * @return int The current energy level.
     */
    int getEnergyLevel() const { return energyLevel_; }

    /**
     * @brief Sets the energy level of the platform.
     * @param energyLevel The new energy level to set.
     */
    void setEnergyLevel(int energyLevel) { energyLevel_ = energyLevel; }

    /**
     * @brief Gets the maximum energy level of the platform.
     * @return int The maximum energy level.
     */
    int getMaxEnergyLevel() const {return maxEnergyLevel_;}

    /**
     * @brief Gets the number of slots available for modules.
     * @return size_t The number of slots.
     */
    size_t getSlotCount() const { return slotCount_; }

    /**
     * @brief Gets the environment the platform is in.
     * @return Environment* Pointer to the environment.
     */
    virtual Environment* getEnvironment() const = 0;

    /**
     * @brief Gets the current position of the platform.
     * @return Pair The current position coordinates.
     */
    virtual Pair getPosition() const = 0;

    /**
     * @brief Updates the platform's state based on spotted suspects.
     * @param spottedSuspects Vector of pointers to spotted suspects.
     */
    virtual void iterate() = 0;

    /**
     * @brief Gets the list of modules attached to the platform.
     * @return std::vector<std::shared_ptr<Module>> The list of modules.
     */
    std::vector<std::shared_ptr<Module>> getModules() const {return modules_;}

    /**
     * @brief Installs a module onto the platform.
     * @param module Pointer to the module to install.
     * @throws std::runtime_error if the module is not attachable to the platform.
     */
    void installModule(std::shared_ptr<Module> module);

    /**
     * @brief Finds a module of a specific type attached to the platform.
     * @tparam T The type of the module to find.
     * @return T* Pointer to the found module, or nullptr if not found.
     */
    template<typename T>
    T* findModuleOfType() const {
        auto modules = getModules();
        for (const auto& module : modules)
            if (T* typed_module = dynamic_cast<T*>(module.get()))
                return typed_module;
        return nullptr;
    }
};