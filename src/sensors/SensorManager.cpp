#include "SensorManager.h"

SensorManager::SensorManager(byte tempPin, byte gasPin, byte potPin)
    : _tempPin(tempPin), _gasPin(gasPin), _potPin(potPin) {}

void SensorManager::begin()
{
    // I pin analogici non richiedono pinMode in input
}
float SensorManager::getTemperature()
{
    int sensorVal = analogRead(_tempPin);
    float voltage = (sensorVal / 1024.0) * 5.0;
    float temperatureC = (voltage - 0.5) * 100.0;
    return temperatureC;
}
int SensorManager::getGasValue()
{
    return analogRead(_gasPin);
}
float SensorManager::getSetpoint()
{
    int potVal = analogRead(_potPin);
    // Mappa il valore del potenziometro (0-1023) a un range di temperatura (es. 20-40°C)
    return map(potVal, 0, 1023, 20, 40);
}