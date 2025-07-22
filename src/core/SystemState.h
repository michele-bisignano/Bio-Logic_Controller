#pragma once

#include "StateType.h"
#include "../sensors/SensorManager.h"
#include "../controllers/ActuatorController.h"
#include "../display/DisplayManager.h"
/**
 * @class SystemState
 * @brief Manages the main logic and state machine of the fermentation chamber.
 *
 * This class is the core of the controller. It orchestrates all other components,
 * implements the Finite State Machine (FSM), and handles emergency conditions.
 */
class SystemState
{
public:
    /**
     * @brief Constructs the SystemState manager.
     * @param sm A reference to the SensorManager instance.
     * @param ac A reference to the ActuatorController instance.
     * @param dm A reference to the DisplayManager instance.
     */
    SystemState(SensorManager &sm, ActuatorController &ac, DisplayManager &dm);

    /**
     * @brief Initializes the system state and dependent components.
     */
    void begin();

    /**
     * @brief The main update loop for the system logic.
     */
    void update();

    /**
     * @brief An ISR-safe method to trigger the hardware emergency stop.
     */
    void triggerEmergencyStop();

private:
    // --- Component References ---
    SensorManager &sensorManager;
    ActuatorController &actuatorController;
    DisplayManager &displayManager;
    // The DebouncedButton reference has been removed.

    // --- State Machine ---
    States::Type _currentState;
    States::Type _stateBeforeEmergency;
    

    // --- State Handlers (Private Methods) ---
    void handleStandby();
    void handlePreheating();
    void handleMaintaining();
    void handleEmergencyStop();

    // --- Display Management ---
    void updateDisplay(String state, float currentTemp, float setpoint, int gasValue);

    // --- Member Variables ---
    float _setpoint;
    int _gasValue;

    // Predictive Control & Timing
    unsigned long _lastUpdateTime;
    float _lastTemperature;
    float _temperatureDerivative;
    unsigned long _heatingPulseStartTime;

    // Siren Management
    bool _sirenShouldBeActive;

    bool _hwEmergencyMessageDisplayed; // Flag to prevent LCD flickering

    // Previous Display State (for optimization)
    float _previousTemperaturePrinted = 0.0;
    float _previousSetpointPrinted = 0.0;
    int _previousGasValuePrinted = 0;
    String _previousStatePrinted = "";
};
