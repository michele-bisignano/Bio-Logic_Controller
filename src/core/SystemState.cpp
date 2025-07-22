#include "SystemState.h"

// === CONSTANTS ===
const int LOW_EMERGENCY_GAS_THRESHOLD = 400;
const int HIGH_EMERGENCY_GAS_THRESHOLD = 700;
const float TEMPERATURE_HYSTERESIS = 0.5;
const float PREDICTIVE_DERIVATIVE_THRESHOLD = -0.05;
const unsigned long HEATING_PULSE_DURATION_MS = 2000;
const float DERIVATIVE_CALCULATION_INTERVAL_S = 2.0;

// === CONSTRUCTOR ===
SystemState::SystemState(SensorManager &sm, ActuatorController &ac, DisplayManager &dm)
    : sensorManager(sm),
      actuatorController(ac),
      displayManager(dm),
      _currentState(States::Type::STANDBY),
      _stateBeforeEmergency(States::Type::STANDBY),
      _sirenShouldBeActive(false),
      _hwEmergencyMessageDisplayed(false)
{
    // The acknowledgeButton has been removed from the initializer list.
}

// === BEGIN ===
void SystemState::begin()
{
    _currentState = States::Type::STANDBY;
    _stateBeforeEmergency = States::Type::STANDBY;
    _lastUpdateTime = millis();
    _lastTemperature = sensorManager.getTemperature();
    _sirenShouldBeActive = false;
    _hwEmergencyMessageDisplayed = false; 
}

// === HARDWARE EMERGENCY TRIGGER (ISR-SAFE) ===
void SystemState::triggerEmergencyStop()
{
    _currentState = States::Type::EMERGENCY_STOP;
}

// === UPDATE (THE CORE LOGIC LOOP) ===
void SystemState::update()
{
    // 1. HANDLE UNRECOVERABLE LOCK STATE (HIGHEST PRIORITY)
    if (_currentState == States::Type::EMERGENCY_STOP)
    {
        handleEmergencyStop();
        return; // Halts all further execution.
    }

    // The user input check for the acknowledge button has been removed.

    // 2. CHECK FOR GAS EMERGENCY (SECOND PRIORITY)
    _gasValue = sensorManager.getGasValue();
    bool isGasEmergency = (_gasValue >= HIGH_EMERGENCY_GAS_THRESHOLD);

    static bool wasInEmergency = false;

    if (isGasEmergency)
    {
        if (!wasInEmergency)
        {
            _stateBeforeEmergency = _currentState;
            wasInEmergency = true;
        }

        // Override normal operation for the emergency.
        actuatorController.setStatusHeater(false);
        actuatorController.setStatusGreenLED(false);
        actuatorController.setStatusRedLED(true);

        // Siren is activated if the gas level is high. It will remain active
        // until the gas level drops.
        if (_gasValue >= HIGH_EMERGENCY_GAS_THRESHOLD)
        {
            _sirenShouldBeActive = true;
        }

        updateDisplay("GAS WARNING!", sensorManager.getTemperature(), sensorManager.getSetpoint(), _gasValue);
    }
    else
    {
        // 3. NORMAL OPERATING LOGIC (THIRD PRIORITY)
        if (wasInEmergency)
        {
            _currentState = _stateBeforeEmergency;
            wasInEmergency = false;
        }

        // This section only runs if there are no active gas emergencies.
        actuatorController.setStatusGreenLED(true);
        actuatorController.setStatusRedLED(false);
        _sirenShouldBeActive = false; // Reset the siren request when conditions are normal.

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
        }
    }

    // 4. FINAL ACTUATOR UPDATE
    actuatorController.setSirenState(_sirenShouldBeActive);
    actuatorController.update();
}

// === STATE HANDLERS ===
// These remain unchanged from the previous version, as they were already correct.

void SystemState::handleStandby()
{
    actuatorController.setStatusGreenLED(false);
    actuatorController.setStatusRedLED(false);
    actuatorController.setStatusHeater(false);
    _lastTemperature = sensorManager.getTemperature();
    _setpoint = sensorManager.getSetpoint();
    updateDisplay(States::toString(States::Type::STANDBY), _lastTemperature, _setpoint, _gasValue);

    if (_lastTemperature < _setpoint)
    {
        _currentState = States::Type::PREHEATING;
    }
}

void SystemState::handlePreheating()
{
    actuatorController.setStatusGreenLED(false);
    actuatorController.setStatusRedLED(true);
    actuatorController.setStatusHeater(true);
    float currentTemperature = sensorManager.getTemperature();
    float setpoint = sensorManager.getSetpoint();
    updateDisplay(States::toString(States::Type::PREHEATING), currentTemperature, setpoint, _gasValue);

    if (currentTemperature >= setpoint)
    {
        _currentState = States::Type::MAINTAINING;
        _lastUpdateTime = millis();
        _lastTemperature = currentTemperature;
        actuatorController.setStatusHeater(false);
    }
}

void SystemState::handleMaintaining()
{
    actuatorController.setStatusGreenLED(true);
    actuatorController.setStatusRedLED(false);

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

    if (sensorManager.getTemperature() < sensorManager.getSetpoint() - TEMPERATURE_HYSTERESIS)
    {
        _currentState = States::Type::PREHEATING;
    }

    updateDisplay(States::toString(States::Type::MAINTAINING), sensorManager.getTemperature(), sensorManager.getSetpoint(), _gasValue);
}

void SystemState::handleEmergencyStop()
{
    actuatorController.setStatusHeater(false);
    actuatorController.setStatusGreenLED(false);
    actuatorController.setStatusRedLED(true);
    actuatorController.setSirenState(true);

    if (!_hwEmergencyMessageDisplayed)
    {
        displayManager.displayEmergency("HW STOP ACTIVATED");
        _hwEmergencyMessageDisplayed = true;
    }
}

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
        displayManager.displayStatus(state, currentTemp, setpoint, gasValue);
    }
}