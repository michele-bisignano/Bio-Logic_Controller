#include "SensorManager.h"

SensorManager::SensorManager(byte tempPin, byte gasPin, byte potPin)
    : _tempPin(tempPin), _gasPin(gasPin), _potPin(potPin) {}

void SensorManager::begin()
{
    // Initialize the lastSetpoint value
    _lastPotReadTime = millis();
    _lastSetpoint = getSetpoint();
}

float SensorManager::getTemperature()
{
    int sensorVal = analogRead(_tempPin);
    float voltage = (sensorVal / 1024.0) * 5.0;
    float temperature = (voltage - 0.5) * 100.0;
    return temperature;
}
int SensorManager::getGasValue()
{
    return analogRead(_gasPin);
}

float SensorManager::getSetpoint()
{
    // Check if enough time has passed since the last read
    if (millis() - _lastPotReadTime >= POT_READ_INTERVAL_MS)
    {
        // It's time  to read the potentiometer again
        _lastPotReadTime = millis();

        // Read the physical value and update our cache
        int potVal = analogRead(_potPin);
        _lastSetpoint = map(potVal, 0, 1023, MIN_SETTABLE_TEMPERATURE, MAX_SETTABLE_TEMPERATURE);
    }

    // ALWAYS return the cached value.
    // This value will be fresh only if the interval has expired,
    // otherwise it will be the last valid value read.
    return _lastSetpoint;
}