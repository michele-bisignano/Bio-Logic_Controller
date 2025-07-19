#pragma once

#include <Arduino.h>

// --- constexprants to configure the siren sound ---
constexpr int SIREN_MIN_FREQUENCY = 500;  // The lowest tone of the siren (in Hz)
constexpr int SIREN_MAX_FREQUENCY = 1500; // The highest tone of the siren (in Hz)
constexpr int SIREN_FREQUENCY_STEP = 25;  // How much to change the frequency on each step
constexpr int SIREN_UPDATE_INTERVAL_MS = 15; // Time between frequency changes (in milliseconds)

/**
 * @brief Manages all output actuators for the fermentation chamber.
 * @details This class provides a high-level interface to control physical
 *          outputs like the heating element (via a transistor) and the status LEDs.
 *          It abstracts away the low-level digitalWrite calls.
 */
class ActuatorController
{
public:
    /**
     * @brief constexprructs the ActuatorController.
     * @param transistorPin The digital pin connected to the transistor module.
     * @param greenLedPin The digital pin connected to the green LED.
     * @param redLedPin The digital pin connected to the red LED.
     */
    ActuatorController(byte transistorPin, byte greenLedPin, byte redLedPin, byte piezoPin);

    /**
     * @brief Initializes the pins for the actuators.
     * @details Sets the pinMode for all controlled pins to OUTPUT.
     */
    void begin();

    /**
     * @brief Turns the heating element on or off.
     * @param active Set to true to activate the heater, false to deactivate it.
     */
    // void setHeater(bool active);

    /**
     * @brief Sets the state of the green LED.
     * @param active Set to true to turn on the green LED, false to turn it off.
     */
    void setStatusGreen(bool active);

    /**
     * @brief Sets the state of the red LED.
     * @param active Set to true to turn on the red LED, false to turn it off.
     */
    void setStatusRed(bool active);

    /**
     * @brief Sets the state of the piezo buzzer.
     * @param active Set to true to turn on the piezo buzzer, false to turn it off.
     */
    void setSirenState(bool active);

    /**
     * @brief Updates the state of time-based actuators, like the siren.
     * @note This is a non-blocking function that must be called on every iteration
     *       of the main Arduino loop(). It manages the frequency sweep of the siren
     *       without using any delay(), allowing other code to run concurrently.
     */
    void update();


private:
    byte _transistorPin;
    byte _greenLedPin;
    byte _redLedPin;
    byte _piezoPin;

    // State variables for the siren
    bool _isSirenActive;
    unsigned long _lastSirenUpdateTime;
    int _currentSirenFrequency;
    bool _isSirenSweepingUp;

    // pivate methods for internal logic
    void updateSirenTone();

};