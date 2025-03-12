#pragma once

#include "../common_types.h"

#include <shared_mutex>

class Module;
class Environment;
class AI;

/**
 * @class Platform
 * @brief Class representing a platform that can hold modules and interact with the environment.
 */
class Platform : public std::enable_shared_from_this<Platform> { // это для shared_from_this
protected:
    std::string description_ {};
    int energyLevel_ = 0;
    int maxEnergyLevel_ = 0;
    size_t slotCount_ = 0;
    std::vector<std::shared_ptr<Module>> modules_ {};
    /**
     * @brief Constructs a Platform object.
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
     * @return std::weak_ptr<Environment> Pointer to the environment.
     * 
     * This method is virtual and must be overridden in derived classes.
     * @virtual
     */
    virtual std::weak_ptr<Environment> getEnvironment() const = 0;

    /**
     * @brief Gets the current position of the platform.
     * @return Pair The current position coordinates.
     * 
     * This method is virtual and must be overridden in derived classes.
     * @virtual
     */
    virtual Pair getPosition() const = 0;

    /**
     * @brief Updates the platform's state based on spotted suspects.
     * 
     * This method is virtual and must be overridden in derived classes.
     * @virtual
     */
    virtual void iterate() = 0;

    /**
     * @brief Pauses the platform's modules.
     */
    void pause();

    /**
     * @brief Resumes the platform's modules.
     */
    void resume();

    /**
     * @brief Gets the AI associated with the platform.
     * @return std::weak_ptr<AI> Pointer to the AI.
     * 
     * This method is virtual and must be overridden in derived classes.
     * @virtual
     */
    virtual std::weak_ptr<AI> getAI() const = 0;

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
     * @brief Gets a shared pointer to the platform.
     * @return std::shared_ptr<Platform> A shared pointer to the platform.
     */
    std::shared_ptr<Platform> getSharedPtr() { return shared_from_this(); }

    /**
     * @brief Finds a module of a specific type attached to the platform.
     * @tparam T The type of the module to find.
     * @return T* Pointer to the found module, or nullptr if not found.
     */
    template<typename T>
    std::shared_ptr<T> findModuleOfType() const {
        auto modules = getModules();
        for (const auto& module : modules)
            if (auto typed_module = std::dynamic_pointer_cast<T>(module))
                return typed_module;
        return nullptr;
    }
};