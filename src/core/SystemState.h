#pragma once

/*
#include "SensorManager.h"
#include "ActuatorController.h"
#include "DisplayManager.h"
*/

constexpr int LOW_EMERGENCY_GAS_THRESHOLD = 600;
constexpr int HIGH_EMERGENCY_GAS_THRESHOLD = 750;

/**
 * @brief Defines the possible operational states of the system.
 */
enum State {
    STANDBY,
    PREHEATING,
    MAINTAINING,
    EMERGENCY_STOP
};

/**
 * @brief Manages the core logic and Finite State Machine (FSM) of the controller.
 * @details This class is the "brain" of the application. It orchestrates all other
 *          components (sensors, actuators, display) based on the current state and
 *          implements the control logic, including the predictive temperature control.
 */
class SystemState {
public:
    /**
     * @brief Constructs the SystemState manager.
     */
    SystemState();

    /**
     * @brief Initializes the state machine and links it to other manager objects.
     * @param sm A pointer to the global SensorManager instance.
     * @param ac A pointer to the global ActuatorController instance.
     * @param dm A pointer to the global DisplayManager instance.
     */
    void begin(SensorManager* sm, ActuatorController* ac, DisplayManager* dm);

    /**
     * @brief The main update function, designed to be called in the main loop.
     * @details Executes the logic corresponding to the current state of the FSM.
     */
    void update();

    /**
     * @brief A special, interrupt-safe method to trigger the emergency stop state.
     * @note This method is designed to be called from an Interrupt Service Routine (ISR).
     *       It is lightweight and only sets a flag to be handled in the main loop.
     */
    void triggerEmergencyStop();

private:
    State _currentState;
    
    // Pointers to the manager objects
    SensorManager* _sensorManager;
    ActuatorController* _actuatorController;
    DisplayManager* _displayManager;

    // Variables for predictive control logic
    float _lastTemp;
    unsigned long _lastTempCheckTime;

    // Private methods to handle the logic for each state
    void handleStandby();
    void handlePreheating();
    void handleMaintaining();
    void handleEmergencyStop();
};
