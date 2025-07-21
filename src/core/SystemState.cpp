#include "SystemState.h"
#include <Arduino.h>

// ======================================================================================
// --- CONFIGURATION AND TUNING CONSTANTS ---
// ======================================================================================
const int LOW_EMERGENCY_GAS_THRESHOLD = 400;
const int HIGH_EMERGENCY_GAS_THRESHOLD = 700;
const float TEMPERATURE_HYSTERESIS = 0.5;
const float PREDICTIVE_DERIVATIVE_THRESHOLD = -0.05;
const unsigned long HEATING_PULSE_DURATION_MS = 2000;
const unsigned long DISPLAY_UPDATE_INTERVAL_MS = 1500;
const float DERIVATIVE_CALCULATION_INTERVAL_S = 2.0;

SystemState::SystemState(SensorManager &sm, ActuatorController &ac, DisplayManager &dm)
    : sensorManager(sm), actuatorController(ac), displayManager(dm),
      _currentState(States::Type::STANDBY), _lastUpdateTime(0), _lastTemperature(0.0),
      _temperatureDerivative(0.0), _heatingPulseStartTime(0) {}

void SystemState::begin()
{
    _currentState = States::Type::STANDBY;
    _lastUpdateTime = millis();
    _lastTemperature = sensorManager.getTemperature();
}

void SystemState::update()
{
    checkAndHandleEmergencies();

    switch (_currentState)
    {
    case States::Type::STANDBY:
        handleStandby();
        break;
    case States::Type::PREHEATING:
        handlePreheating();
        break;
    case States::Type::MAINTAINING:
        handleMaintaining();
        break;
    case States::Type::EMERGENCY_STOP:
        handleEmergencyStop();
        break;
        // No default case needed here, as checkAndHandleEmergencies covers all states,
        // and an invalid state in the variable itself would be a memory corruption issue.
    }
}

void SystemState::triggerEmergencyStop()
{
    // This ISR-callable method must be extremely fast.
    actuatorController.setStatusHeater(false);
    actuatorController.setSirenState(true);
    actuatorController.setStatusRedLED(true);
    _currentState = States::Type::EMERGENCY_STOP;
}

void SystemState::checkAndHandleEmergencies()
{
    // This check runs every cycle, regardless of the FSM state.
    // It has priority over normal state operations for safety.
    if (_currentState == States::Type::EMERGENCY_STOP)
    {
        return; // The hardware interrupt state is final.
    }

    _gasValue = sensorManager.getGasValue();

    if (_gasValue >= HIGH_EMERGENCY_GAS_THRESHOLD)
    {
        actuatorController.setStatusGreenLED(false);
        actuatorController.setStatusRedLED(true);
        actuatorController.setSirenState(true);
    }
    else if (_gasValue >= LOW_EMERGENCY_GAS_THRESHOLD)
    {
        actuatorController.setStatusGreenLED(false);
        actuatorController.setStatusRedLED(true);
        actuatorController.setSirenState(false);
    }
    else
    {
        // If gas level is normal, ensure the siren is off.
        // The control of the LED color is returned to the state handlers.
        actuatorController.setSirenState(false);
    }
}

void SystemState::handleStandby()
{
    actuatorController.setStatusHeater(false);

    // The LED is green only if there is no gas emergency.
    if (sensorManager.getGasValue() < LOW_EMERGENCY_GAS_THRESHOLD)
    {
        actuatorController.setStatusRedLED(false);
        actuatorController.setStatusGreenLED(true);
    }

    _lastTemperature = sensorManager.getTemperature();
    _setpoint = sensorManager.getSetpoint();
    updateDisplay(States::toString(States::Type::STANDBY), _lastTemperature, _setpoint, _gasValue);

    if (_setpoint > _lastTemperature + TEMPERATURE_HYSTERESIS)
    {
        _currentState = States::Type::PREHEATING;
    }
}

void SystemState::handlePreheating()
{
    actuatorController.setStatusHeater(false);

    // The LED is red for heating, but the gas emergency check can override this.
    if (sensorManager.getGasValue() < LOW_EMERGENCY_GAS_THRESHOLD)
    {
        actuatorController.setStatusGreenLED(false);
        actuatorController.setStatusRedLED(true);
    }

    float currentTemperature = sensorManager.getTemperature();
    float setpoint = sensorManager.getSetpoint();
    updateDisplay(States::toString(States::Type::PREHEATING), currentTemperature, setpoint, _gasValue);

    if (currentTemperature >= setpoint)
    {
        _currentState = States::Type::MAINTAINING;
        _lastUpdateTime = millis();
        _lastTemperature = currentTemperature;
        actuatorController.setStatusHeater(true);
    }
}

void SystemState::handleMaintaining()
{
    // The LED is green for stable, but the gas emergency check can override this.
    if (sensorManager.getGasValue() < LOW_EMERGENCY_GAS_THRESHOLD)
    {
        actuatorController.setStatusRedLED(false);
        actuatorController.setStatusGreenLED(true);
    }

    // --- Predictive Control Logic ---
    unsigned long now = millis();
    if (_heatingPulseStartTime > 0)
    {
        if (now - _heatingPulseStartTime >= HEATING_PULSE_DURATION_MS)
        {
            actuatorController.setStatusHeater(false);
            _heatingPulseStartTime = 0;
        }
    }
    else
    {
        if (now - _lastUpdateTime >= (DERIVATIVE_CALCULATION_INTERVAL_S * 1000))
        {
            float dt_seconds = (now - _lastUpdateTime) / 1000.0f;
            float currentTemperature = sensorManager.getTemperature();
            _temperatureDerivative = (currentTemperature - _lastTemperature) / dt_seconds;

            _lastUpdateTime = now;
            _lastTemperature = currentTemperature;

            if (_temperatureDerivative < PREDICTIVE_DERIVATIVE_THRESHOLD && currentTemperature < sensorManager.getSetpoint())
            {
                actuatorController.setStatusHeater(true);
                _heatingPulseStartTime = now;
            }
        }
    }

    // --- Fallback Safety Transition ---
    if (sensorManager.getTemperature() < sensorManager.getSetpoint() - TEMPERATURE_HYSTERESIS)
    {
        _currentState = States::Type::PREHEATING;
        return;
    }

    updateDisplay(States::toString(States::Type::MAINTAINING), _lastTemperature, _setpoint, _gasValue);
}

void SystemState::handleEmergencyStop()
{
    // This is a terminal state. Actuators were already handled by triggerEmergencyStop().
    // We just keep displaying the emergency message.
    displayManager.displayEmergency("High gas or temp");
}

/**
 * @brief Updates the display with the current system status, only if changes are detected.
 *
 * @param state     A string representing the current FSM state (e.g., "STANDBY", "MAINTAINING").
 * @param currentTemp The current temperature reading from the sensor.
 * @param setpoint  The target temperature set by the user.
 * @param gasValue  The latest gas sensor reading.
 *
 * @details This function compares the current values with the previously displayed values.
 * If any parameter has changed, it updates the internal cached values and refreshes the display
 * via the DisplayManager. If all values remain unchanged, the function returns immediately
 * to avoid unnecessary updates and flickering on the display.
 */
void SystemState::updateDisplay(String state, float currentTemp, float setpoint, int gasValue)
{
    if (_previousTemperaturePrinted != currentTemp ||
        _previousSetpointPrinted != setpoint ||
        _previousGasValuePrinted != gasValue ||
        _previousStatePrinted != state)
    {
        _previousTemperaturePrinted = currentTemp;
        _previousSetpointPrinted = setpoint;
        _previousGasValuePrinted = gasValue;
        _previousStatePrinted = state;
    }
    else
    {
        return; // No change, skip display update
    }
    displayManager.displayStatus(state, currentTemp, setpoint, gasValue);
}