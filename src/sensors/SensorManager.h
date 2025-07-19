#pragma once

#include <Arduino.h>

/**
 * @brief Manages all sensor readings for the fermentation chamber.
 * @details This class centralizes all sensor-related code. It is responsible
 *          for reading raw values and converting them into meaningful data,
 *          such as temperature in Celsius and the desired setpoint.
 */
class SensorManager {
public:
    /**
     * @brief Constructs the SensorManager.
     * @param tempPin The analog pin connected to the TMP36 temperature sensor.
     * @param gasPin The analog pin connected to the MQ-3 gas sensor.
     * @param potPin The analog pin connected to the potentiometer for setpoint adjustment.
     */
    SensorManager(byte tempPin, byte gasPin, byte potPin);

    /**
     * @brief Initializes the sensor pins.
     * @details Sets the pinMode for all sensor pins to INPUT.
     */
    void begin();

    /**
     * @brief Reads the temperature sensor and converts the value to Celsius.
     * @return The current temperature in degrees Celsius (float).
     */
    float getTemperature();

    /**
     * @brief Reads the raw analog value from the gas sensor.
     * @return An integer value from 0 to 1023 representing gas concentration.
     */
    int getGasValue();

    /**
     * @brief Reads the potentiometer and converts it to a target temperature.
     * @note The mapping from 0-1023 to a temperature range is defined internally.
     * @return The desired setpoint temperature in degrees Celsius (float).
     */
    float getSetpoint();

private:
    byte _tempPin; // Pin for the temperature
    byte _gasPin;  // Pin for the gas sensor
    byte _potPin;  // Pin for the potentiometer
};