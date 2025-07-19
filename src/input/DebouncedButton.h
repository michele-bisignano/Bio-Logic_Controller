#pragma once

#include <Arduino.h>

/**
 * @class DebouncedButton
 * @brief A robust, reusable class to handle a physical button with software debouncing.
 *
 * @details This class solves the common problem of "contact bouncing" in mechanical
 *          buttons. When a physical button is pressed, its contacts can bounce
 *          and create multiple high/low signal transitions in a few milliseconds.
 *          This class filters out this noise using a non-blocking timer based on
 *          millis(), ensuring that a single physical press is registered as a
 *          single, clean event in the software.
 *
 *          The logic is self-contained. The user only needs to instantiate the
 *          class and call the isPressed() method in the main loop.
 */
class DebouncedButton
{
public:
    /**
     * @brief Constructs a new DebouncedButton object.
     *
     * @param pin The digital pin number to which the button is connected.
     * @param debounceDelay The time in milliseconds to wait for the signal to
     *                      stabilize. A value of 50ms is typically effective.
     */
    DebouncedButton(byte pin, unsigned long debounceDelay = 50);

    /**
     * @brief Initializes the button. Must be called once in the setup() function.
     *
     * @details This method configures the specified pin as an input with an
     *          internal pull-up resistor enabled (INPUT_PULLUP). This is the
     *          standard and recommended way to wire a button to an Arduino,
     *          as it eliminates the need for an external pull-up resistor.
     *          The button should be wired to connect the pin to GND when pressed.
     */
    void begin();

    /**
     * @brief Checks if the button was pressed. This is the main method to use in the loop.
     *
     * @details This method returns true only for the single program cycle immediately
     *          following a confirmed button press. It detects the "falling edge"
     *          of the signal (the transition from HIGH to LOW) after it has been
     *          de-bounced. It will not return true again until the button is
     *          released and pressed again.
     *
     * @return bool - True if a new press event has just occurred, false otherwise.
     */
    bool isPressed();

private:
    /**
     * @brief Internal helper method to update the button's state.
     * @details This method is called automatically by isPressed(). It reads the
     *          physical pin and updates the internal state machine based on the
     *          debouncing logic. It is private because the user should not
     *          call it directly.
     */
    void checkPinState();

    // --- Member Variables ---

    byte _pin;                          ///< The Arduino digital pin number for the button.
    unsigned long _debounceDelay;       ///< The duration (in ms) the signal must be stable to be considered valid.

    // Corrected state variables
    int _currentState;          // The stable, confirmed state of the button
    int _lastFlickerableState;  // The last read state, even if noisy
    unsigned long _lastDebounceTime;
};