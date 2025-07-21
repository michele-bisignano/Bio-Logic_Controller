#pragma once

#include "StateType.h"
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
class SystemState {
public:
    /**
     * @brief Constructor for the SystemState class.
     * @param sm A reference to a SensorManager instance. Using a reference (&)
     *           implements the Dependency Injection principle.
     * @param ac A reference to an ActuatorController instance.
     * @param dm A reference to a DisplayManager instance.
     */
    SystemState(SensorManager& sm, ActuatorController& ac, DisplayManager& dm);

    /**
     * @brief Initializes the system state and its dependent components.
     * Should be called once within the main `setup()` function.
     */
    void begin();

    /**
     * @brief Executes the main logic cycle for the system.
     * This method must be called repeatedly within the main `loop()` function.
     */
    void update();

    /**
     * @brief Triggers the emergency stop state via an ISR.
     * This method must be extremely fast and non-blocking. It immediately deactivates
     * critical actuators and forces a transition to the EMERGENCY_STOP state.
     */
    void triggerEmergencyStop();

private:
    // --- Hardware Module References (Dependency Injection) ---
    SensorManager& sensorManager;
    ActuatorController& actuatorController;
    DisplayManager& displayManager;

    // --- Internal State ---
    /** @brief Holds the current state of the Finite State Machine. */
    States::Type _currentState;

    // --- State and Emergency Handlers ---
    void handleStandby();
    void handlePreheating();
    void handleMaintaining();
    void handleEmergencyStop();
    
    /**
     * @brief Checks for non-interrupt driven emergencies like gas leaks.
     * This method runs on every loop cycle to ensure constant monitoring,
     * overriding normal actuator states if an emergency is detected.
     */
    void checkAndHandleEmergencies();

    /**
     * @brief Updates the display with the current system status.
     * This method is called periodically to refresh the display content
     * without causing flickering or excessive I2C traffic.
     */
    void updateDisplay(String state, float currentTemp, float setpoint, int gasValue);

    // --- Predictive Control Variables ---
    /** @brief Timestamp (in ms) of the last derivative calculation. */
    unsigned long _lastUpdateTime;
    /** @brief Temperature recorded at the last update, used to calculate the derivative. */
    float _lastTemperature;
    /** @brief The target temperature set by the user, used in predictive control logic. */
    float _setpoint;
    /** @brief The calculated rate of temperature change (degrees C per second). */
    float _temperatureDerivative;
    /** @brief The last gas sensor reading, used for emergency checks. */
    int _gasValue;
    /** @brief Timestamp (in ms) marking the start of a heating pulse. Is 0 if no pulse is active. */
    unsigned long _heatingPulseStartTime;
    // --- previous Display State Variables ---
    float _previousTemperaturePrinted, _previousSetpointPrinted;
    int _previousGasValuePrinted;
    String _previousStatePrinted;

};