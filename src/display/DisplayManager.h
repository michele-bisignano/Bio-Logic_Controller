
#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

/**
 * @brief Manages all interactions with the 16x2 I2C LCD screen.
 * @details This class encapsulates the LiquidCrystal_I2C library to provide
 *          a clean and simple interface for displaying system status, sensor
 *          data, and messages relevant to the fermentation process.
 */
class DisplayManager {
public:
    /**
     * @brief Constructs the DisplayManager.
     * @param lcdAddr The I2C address of the LCD module.
     * @param lcdCols The number of columns of the display (e.g., 16).
     * @param lcdRows The number of rows of the display (e.g., 2).
     */
    DisplayManager(uint8_t lcdAddr, uint8_t lcdCols, uint8_t lcdRows);

    /**
     * @brief Initializes the LCD screen.
     * @details Calls the init() and backlight() methods of the LCD library.
     */
    void begin();

    /**
     * @brief Displays the main status screen.
     * @details Shows the current state, current temperature, and setpoint temperature.
     * @param state A string representing the current system state (e.g., "MAINTAINING").
     * @param currentTemp The current measured temperature.
     * @param setpointTemp The target temperature.
     */
    void printStatus(const String& state, float currentTemp, float setpointTemp);

    /**
     * @brief Displays a custom two-line message on the screen.
     * @details Useful for alerts, boot messages, or emergency states.
     * @param line1 The text to display on the first row.
     * @param line2 The text to display on the second row.
     */
    void printMessage(const String& line1, const String& line2);

private:
    LiquidCrystal_I2C _lcd;
};
