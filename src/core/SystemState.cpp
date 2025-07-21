#include "SystemState.h"
#include "StateType.h"
#include <Arduino.h> // Required for the millis() function

/** @brief ADC value from the MQ-3 sensor to trigger a low-level emergency (red light only).
 *  This value should be calibrated experimentally based on the sensor's response
 *  in clean air versus a low concentration of the target gas (e.g., ethanol). */
const int LOW_EMERGENCY_GAS_THRESHOLD = 400;

/** @brief ADC value from the MQ-3 sensor to trigger a high-level emergency (red light + siren).
 *  Represents a gas concentration considered critical. */
const int HIGH_EMERGENCY_GAS_THRESHOLD = 700;

/** @brief Hysteresis in degrees Celsius. Prevents rapid, unwanted state switching
 *  (e.g., between MAINTAINING and PREHEATING) when the temperature fluctuates near the setpoint. */
const float TEMPERATURE_HYSTERESIS = 0.5;

/** @brief Derivative threshold (degrees C / second) for predictive control.
 *  If the temperature drops faster than this rate, a heating pulse is triggered.
 *  A more negative value (e.g., -0.1) makes the system less sensitive, while a value
 *  closer to zero (e.g., -0.01) makes it more responsive. */
const float PREDICTIVE_DERIVATIVE_THRESHOLD = -0.05;

/** @brief Duration in milliseconds of a single predictive heating pulse.
 *  A longer value provides more energy but may cause temperature overshoot.
 *  A shorter value is more gentle. */
const unsigned long HEATING_PULSE_DURATION_MS = 2000;

/** @brief Interval in seconds for recalculating the temperature derivative.
 *  A longer interval averages out fluctuations, making the calculation more stable
 *  but less responsive to rapid changes. */
const float DERIVATIVE_CALCULATION_INTERVAL_S = 2.0;

SystemState::SystemState(SensorManager &sm, ActuatorController &ac, DisplayManager &dm)
    : sensorManager(sm), actuatorController(ac), displayManager(dm),
      currentState(States::Type::STANDBY), _lastUpdateTime(0), _lastTemperature(0.0),
      _temperatureDerivative(0.0), _heatingPulseStartTime(0) {}

void SystemState::setup()
{
    currentState = States::Type::STANDBY;
    _lastUpdateTime = millis();
    _lastTemperature = sensorManager.getTemperature();
    _setpoint = sensorManager.getSetpoint();
    displayManager.displayStatus(States::toString(currentState), _lastTemperature, _setpoint, _gasValue);
    delay(1500); // Short pause to allow the boot message to be read.
}

void SystemState::loop()
{
    // 2. Process logic based on the current state.
    switch (currentState)
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
    }
}

void SystemState::triggerEmergencyStop()
{
    // This function is atomic and fast, suitable for an ISR.
    // It acts directly on actuators for an immediate response,
    // bypassing the normal loop cycle.
    actuatorController.setStatusHeater(false);
    actuatorController.setSirenState(true);
    actuatorController.setStatusRedLED(true);
    currentState = States::Type::EMERGENCY_STOP;
}

// ======================================================================================
// --- STATE HANDLER IMPLEMENTATIONS ---
// ======================================================================================

void SystemState::handleStandby()
{
    // GOAL: Keep the system in a safe, low-power state while waiting for instructions.
    // ACTIONS: All main actuators are off. The status LED is green.
    actuatorController.setStatusHeater(false);
    actuatorController.setSirenState(false);
    actuatorController.setStatusGreenLED(true);

    float currentTemperature = sensorManager.getTemperature();
    float setpoint = sensorManager.getSetpoint();
    displayManager.displayStatus(States::toString(currentState), _lastTemperature, _setpoint, _gasValue);

    // TRANSITION: Switch to PREHEATING if the setpoint is raised significantly
    // above the current temperature. Hysteresis prevents spurious transitions.
    if (setpoint > currentTemperature + TEMPERATURE_HYSTERESIS)
    {
        currentState = States::Type::PREHEATING;
    }
}

void SystemState::handlePreheating()
{
    // GOAL: Reach the setpoint temperature as quickly as possible.
    // ACTIONS: The heater is on continuously. The status LED is red to indicate activity.
    actuatorController.setStatusHeater(true);
    actuatorController.setSirenState(false);
    actuatorController.setStatusRedLED(true);

    float currentTemperature = sensorManager.getTemperature();
    float setpoint = sensorManager.getSetpoint();
    displayManager.displayStatus(States::toString(currentState), _lastTemperature, _setpoint, _gasValue);

    // TRANSITION: Switch to MAINTAINING as soon as the temperature reaches or exceeds the setpoint.
    if (currentTemperature >= setpoint)
    {
        currentState = States::Type::MAINTAINING;
        // Initialize variables for predictive control before entering the new state.
        _lastUpdateTime = millis();
        _lastTemperature = currentTemperature;
        actuatorController.setStatusHeater(false);
    }
}

void SystemState::handleMaintaining()
{
    // GOAL: Maintain a stable temperature around the setpoint with high precision.
    float currentTemperature = sensorManager.getTemperature();
    float setpoint = sensorManager.getSetpoint();
    int gasValue = sensorManager.getGasValue();

    // --- 1. Gas Emergency Handling ---
    // Continuously check gas levels and trigger the appropriate alarms.
    if (gasValue >= HIGH_EMERGENCY_GAS_THRESHOLD)
    {
        actuatorController.setStatusRedLED(true);
    }
    else if (gasValue >= LOW_EMERGENCY_GAS_THRESHOLD)
    {
        actuatorController.setStatusRedLED(false);
    }
    else
    {
        actuatorController.setStatusGreenLED(); // If no emergency, the LED is green.
    }

    // --- 2. Predictive Control Logic ---
    unsigned long now = millis();
    if (_heatingPulseStartTime > 0)
    {
        // If a pulse is already active, check if it has finished.
        if (now - _heatingPulseStartTime >= HEATING_PULSE_DURATION_MS)
        {
            actuatorController.setStatusHeater(false);
            _heatingPulseStartTime = 0; // Reset the pulse timer.
        }
    }
    else
    {
        // If no pulse is active, evaluate whether to start a new one.
        if (now - _lastUpdateTime >= (DERIVATIVE_CALCULATION_INTERVAL_S * 1000))
        {
            float dt_seconds = (now - _lastUpdateTime) / 1000.0f;
            // Calculate derivative: (current_temp - last_temp) / elapsed_time
            _temperatureDerivative = (currentTemperature - _lastTemperature) / dt_seconds;

            _lastUpdateTime = now;
            _lastTemperature = currentTemperature;

            // Predictive decision: if temp is dropping too fast and we are below the setpoint,
            // anticipate the heat loss with a pulse.
            if (_temperatureDerivative < PREDICTIVE_DERIVATIVE_THRESHOLD && currentTemperature < setpoint)
            {
                actuatorController.setStatusHeater(true);
                _heatingPulseStartTime = now; // Start the timer for the pulse duration.
            }
        }
    }

    // --- 3. Safety Fallback Transition ---
    // If, despite the predictive logic, the temperature drops too much,
    // return to the preheating state for a more aggressive recovery.
    if (currentTemperature < setpoint - TEMPERATURE_HYSTERESIS)
    {
        currentState = States::Type::PREHEATING;
        return; // Exit to allow the next loop cycle to run the PREHEATING logic.
    }

    displayManager.displayStatus(States::toString(currentState), _lastTemperature, _setpoint, _gasValue);
}

void SystemState::handleEmergencyStop()
{
    // GOAL: Keep the system in a safe shutdown state until reset.
    // ACTIONS: No active operations. Actuators were already disabled by
    // triggerEmergencyStop(). The system just displays the error message.
    displayManager.displayEmergency("High temp or gas");
    // The only way out of this state is a hardware reset.
}