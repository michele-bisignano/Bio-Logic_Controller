#include "DebouncedButton.h"

DebouncedButton::DebouncedButton(byte pin, unsigned long debounceDelay)
    : _pin(pin), _debounceDelay(debounceDelay), _lastDebounceTime(0)
{}


void DebouncedButton::begin()
{
    pinMode(_pin, INPUT_PULLUP);
    // Initialize both states with the current reading
    _lastFlickerableState = digitalRead(_pin);
    _currentState = _lastFlickerableState;
}


bool DebouncedButton::isPressed()
{
    // This function now contains all the logic
    checkPinState(); // The call to update is now internal and private
    
    // Detects the "falling edge": the transition from the stable HIGH to LOW state
    if (_currentState == LOW && _lastFlickerableState == HIGH) {
        return true;
    }
    return false;
}

void DebouncedButton::checkPinState()
{
    int reading = digitalRead(_pin);

    // If the state changes (noise or press), reset the timer
    if (reading != _lastFlickerableState) {
        _lastDebounceTime = millis();
    }

    // If the debounce time has passed, the state is stable
    if ((millis() - _lastDebounceTime) > _debounceDelay) {
        // Update the stable state only if it's different from the current one
        if (reading != _currentState) {
            _currentState = reading;
        }
    }
    
    _lastFlickerableState = reading;
}