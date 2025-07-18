#pragma once


#include <Arduino.h>

/**
 * @brief A reusable class to handle a physical button with software debouncing.
 * @details This class uses the millis() function to filter out electrical noise
 *          (bouncing) from a button press. It ensures that a single physical
 *          press is registered as a single event in the software.
 */
class DebouncedButton {
public:
    /**
     * @brief Constructs the DebouncedButton.
     * @param pin The digital pin the button is connected to.
     */
    DebouncedButton(byte pin);

    /**
     * @brief Initializes the button pin.
     * @details Sets the pin mode to INPUT_PULLUP, assuming the button connects the pin to GND when pressed.
     */
    void begin();

    /**
     * @brief Checks if the button has been pressed.
     * @details This method should be called repeatedly in the main loop.
     * @return Returns true only for the single loop cycle right after the button is confirmed as pressed. Returns false otherwise.
     */
    bool isPressed();

private:
    byte _pin;
    unsigned long _lastDebounceTime;
    bool _lastButtonState;
    bool _buttonState;
    static const unsigned long _debounceDelay = 50; // 50ms debounce time
};