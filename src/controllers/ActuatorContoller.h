#pragma once

#include <Arduino.h>

/**
 * @brief Manages all output actuators for the fermentation chamber.
 * @details This class provides a high-level interface to control physical
 *          outputs like the heating element (via a relay) and the status LEDs.
 *          It abstracts away the low-level digitalWrite calls.
 */
class ActuatorController {
public:
    /**
     * @brief Constructs the ActuatorController.
     * @param relayPin The digital pin connected to the relay module.
     * @param greenLedPin The digital pin connected to the green LED.
     * @param redLedPin The digital pin connected to the red LED.
     */
    ActuatorController(byte relayPin, byte greenLedPin, byte redLedPin);

    /**
     * @brief Initializes the pins for the actuators.
     * @details Sets the pinMode for all controlled pins to OUTPUT.
     */
    void begin();

    /**
     * @brief Turns the heating element on or off.
     * @param on Set to true to activate the heater, false to deactivate it.
     */
    void setHeater(bool on);

    /**
     * @brief Sets the state of the bi-color status LED.
     * @param status The desired status: 0 for OFF, 1 for GREEN (stable), 2 for RED (heating/error).
     */
    void setStatusLed(int status);

private:
    byte _relayPin;
    byte _greenLedPin;
    byte _redLedPin;
};