#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t i2cAddr, uint8_t cols, uint8_t rows)
    : _lcd(i2cAddr, cols, rows)
{}

void DisplayManager::begin()
{
    // Initialize the library
    _lcd.init();
    // Turn on the backlight to make the text visible
    _lcd.backlight();
    // Clear any leftover characters from a previous run
    _lcd.clear();
}

/**
 * @brief clear() method implementation.
 */
void DisplayManager::clear()
{
    _lcd.clear();
}

void DisplayManager::print(const String &line1, const String &line2)
{
    // Clear the screen first to prevent text from overlapping
    _lcd.clear();

    // Set the cursor to the beginning of the first line (column 0, row 0)
    _lcd.setCursor(0, 0);
    _lcd.print(line1);

    // Set the cursor to the beginning of the second line (column 0, row 1)
    _lcd.setCursor(0, 1);
    _lcd.print(line2);
}

void DisplayManager::displayStatus(String state, float currentTemp, float setpoint, int gasValue)
{
    _lcd.clear();

    // --- First Line: Temperature and Setpoint ---
    _lcd.setCursor(0, 0);
    // String(float, precision) approximate the float with precision decimal palces
    String tempStr = "T:" + String(currentTemp, 1); // Format temperature to 1 decimal place
    String setpointStr = "S:" + String(setpoint, 1); // Format setpoint to 1 decimal place
    _lcd.print(tempStr + " " + setpointStr);

    // --- Second Line: System State and Gas Value ---
    _lcd.setCursor(0, 1);
    String stateStr = state;
    // Truncate the state string if it's too long to fit
    if (stateStr.length() > 9)
    {
        stateStr = stateStr.substring(0, 9);
    }
    _lcd.print(stateStr);

    // Format the gas value string
    String gasStr = "G:" + String(gasValue);
    // Calculate the starting position to right-align the text
    int gasCursorPos = 16 - gasStr.length();
    _lcd.setCursor(gasCursorPos, 1);
    _lcd.print(gasStr);
}

void DisplayManager::displayEmergency(const String &message)
{
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("!EMERGENCY STOP!");
    _lcd.setCursor(0, 1);
    _lcd.print(message); // Print the emergency message on the second line
}