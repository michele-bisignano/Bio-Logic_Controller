#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

/**
 * @class DisplayManager
 * @brief Manages all interactions with a 16x2 I2C LCD screen.
 *
 * @details This class acts as a dedicated controller for the LCD, abstracting away
 *          the low-level details of the LiquidCrystal_I2C library. It provides a
 *          clean, high-level interface for other parts of the firmware to print
 *          status messages, sensor data, and alerts without needing to know
 *          about cursor positions or I2C addresses.
 */
class DisplayManager
{
public:
    /**
     * @brief Constructs a new DisplayManager object.
     *          
     *
     * @param i2cAddr The I2C address of the LCD module.
     * @param cols The number of columns the LCD has (e.g., 16).
     * @param rows The number of rows the LCD has (e.g., 2).
     * 
     * @attention The class is designed for I2C based on PCF8574
     */
    DisplayManager(uint8_t i2cAddr, uint8_t cols = 16, uint8_t rows = 2);

    /**
     * @brief Initializes the LCD display. Must be called once in the setup() function.
     *
     * @details This method initializes the I2C communication with the LCD, turns on
     *          the backlight, and clears the screen to ensure it's ready for use.
     */
    void begin();

    /**
     * @brief Clears all content from the LCD screen.
     */
    void clear();

    /**
     * @brief Displays a generic two-line message. Ideal for startup or simple alerts.
     *
     * @param line1 The text to be displayed on the first row.
     * @param line2 (Optional) The text to be displayed on the second row. If omitted, the second row remains blank.
     *
     * @attention The display can only show 16 characters per line. Longer strings
     *            will be truncated by the display and will not wrap automatically.
     */
    void print(const String &line1, const String &line2 = "");

    /**
     * @brief Displays the main operational status screen.
     *
     * @details This method formats and displays the key real-time data for the
     *          fermentation chamber: current temperature, setpoint, system state,
     *          and gas sensor reading. It handles formatting, truncation, and
     *          alignment to fit neatly on the 16x2 display.
     *
     * @param state A string representing the current system state (e.g., "MAINTAINING").
     * @param currentTemp The current measured temperature.
     * @param setpoint The target temperature set by the user.
     * @param gasValue The value from the gas sensor.
     */
    void displayStatus(const String &state, float currentTemp, float setpoint, int gasValue);

    /**
     * @brief Displays a critical emergency message, overriding any other content.
     *
     * @param message A short message describing the reason for the emergency state.
     */
    void displayEmergency(const String &message);

private:
    // --- Member Variables ---

    /**
     * @brief The instance of the LiquidCrystal_I2C library.
     * @details This private member variable is the actual object that communicates
     *          with the physical LCD hardware.
     */
    LiquidCrystal_I2C _lcd;
};