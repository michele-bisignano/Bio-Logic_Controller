#pragma once

#include "StateType.h" // Include the state definitions from the new file
#include "../sensors/SensorManager.h"
#include "../controllers/ActuatorController.h"
#include "../display/DisplayManager.h"

/**
 * @class SystemState
 * @brief The orchestrator class that acts as the "brain" of the control system.
 *
 * This class implements the Finite State Machine (FSM) logic and manages the
 * interactions between the various system modules (sensors, actuators, display).
 * Its purpose is to execute the main control loop, implement the predictive
 * control logic, and handle state transitions safely and reliably.
 */
class SystemState
{
public:
    /**
     * @brief Constructor for the SystemState class.
     * @param sm A reference to a SensorManager instance. Using a reference (&)
     *           implements the Dependency Injection principle, making the class
     *           more modular and testable.
     * @param ac A reference to an ActuatorController instance.
     * @param dm A reference to a DisplayManager instance.
     */
    SystemState(SensorManager &sm, ActuatorController &ac, DisplayManager &dm);

    /**
     * @brief Initializes the system state and its dependent components.
     * Should be called once within the main `setup()` function of the Arduino sketch.
     */
    void setup();

    /**
     * @brief Executes the main logic cycle for the system.
     * This method must be called repeatedly within the main `loop()` function.
     * It executes the logic corresponding to the current FSM state.
     */
    void loop();

    /**
     * @brief Triggers the emergency stop state.
     * Designed to be called from an Interrupt Service Routine (ISR), this method
     * must be extremely fast and non-blocking. It immediately deactivates
     * critical actuators and forces a transition to the EMERGENCY_STOP state,
     * ensuring an immediate response to a critical event.
     */
    void triggerEmergencyStop();

private:
    // --- Hardware Module References (Dependency Injection) ---
    SensorManager &sensorManager;
    ActuatorController &actuatorController;
    DisplayManager &displayManager;

    // --- Internal State ---
    /** @brief Holds the current state of the Finite State Machine. */
    States::Type currentState;

        // --- Private State Handlers ---
    /** @brief Contains the logic executed during the STANDBY state. */
    void handleStandby();
    /** @brief Contains the logic executed during the PREHEATING state. */
    void handlePreheating();
    /** @brief Contains the logic executed during the MAINTAINING state. */
    void handleMaintaining();
    /** @brief Contains the logic executed during the EMERGENCY_STOP state. */
    void handleEmergencyStop();

    // --- Predictive Control Variables ---
    /** @brief Timestamp (in ms) of the last derivative calculation. */
    unsigned long _lastUpdateTime;
    /** @brief Temperature recorded at the last update, used to calculate the derivative. */
    float _lastTemperature;
    /** @brief The calculated rate of temperature change (degrees C per second). */
    float _temperatureDerivative;
    /** @brief The target temperature set by the user. */
    float _setpoint;
    /** @brief The current gas sensor reading, used for monitoring purposes. */
    int _gasValue;
    /** @brief Timestamp (in ms) marking the start of a heating pulse. Is 0 if no pulse is active. */
    unsigned long _heatingPulseStartTime;
};