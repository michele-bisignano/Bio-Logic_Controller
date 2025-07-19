#pragma once

#include <Arduino.h>

constexpr int MAX_SETTABLE_TEMPERATURE = 40;        // Maximum temperature setpoint in Celsius
constexpr int MIN_SETTABLE_TEMPERATURE = 20;        // Minimum temperature setpoint in Celsius
constexpr unsigned long POT_READ_INTERVAL_MS = 500; // Interval for reading the potentiometer in ms

/**
 * @brief Manages all sensor readings for the fermentation chamber.
 * @details This class centralizes all sensor-related code. It is responsible
 *          for reading raw values and converting them into meaningful data,
 *          such as temperature in Celsius and the desired setpoint.
 */
class SensorManager
{
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
     * @brief Returns the current setpoint value based on the potentiometer reading.
     *
     * This method implements a time-based polling logic.
     * The potentiometer's value is read from the hardware only if enough
     * time (defined by POT_READ_INTERVAL_MS) has passed since the last read.
     * Otherwise, the last valid cached value is returned.
     *
     * The internal logic performs the following steps:
     * 1. Checks if `millis() - _lastPotReadTime >= POT_READ_INTERVAL_MS`.
     * 2. If true, it reads the raw analog value (0-1023), maps it to the
     *    desired range (20-40), and updates both `_lastSetpoint` and `_lastPotReadTime`.
     * 3. It always returns the value of `_lastSetpoint`, whether it was just updated
     *    or is from a previous read.
     *
     * @return float The setpoint value, which is only updated if the read interval has expired.
     */
    float getSetpoint();

private:
    byte _tempPin; // Pin for the temperature
    byte _gasPin;  // Pin for the gas sensor
    byte _potPin;  // Pin for the potentiometer

    unsigned long _lastPotReadTime; // Timestamp of the last potentiometer read
    float _lastSetpoint;            // Last setpoint value read from the potentiometer
};