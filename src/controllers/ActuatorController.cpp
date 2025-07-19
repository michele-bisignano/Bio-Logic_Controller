#include "ActuatorController.h"

// Constants are implementation details, so they belong in the .cpp file.
constexpr int SIREN_MIN_FREQUENCY = 500;
constexpr int SIREN_MAX_FREQUENCY = 1500;
constexpr int SIREN_FREQUENCY_STEP = 25;
constexpr int SIREN_UPDATE_INTERVAL_MS = 15;

ActuatorController::ActuatorController(byte transistorPin, byte greenLedPin, byte redLedPin, byte piezoPin)
    : _transistorPin(transistorPin),
      _greenLedPin(greenLedPin),
      _redLedPin(redLedPin),
      _piezoPin(piezoPin),
      _isSirenActive(false),
      _lastSirenUpdateTime(0),
      _currentSirenFrequency(SIREN_MIN_FREQUENCY),
      _isSirenSweepingUp(true)
{
}

void ActuatorController::begin()
{
    pinMode(_transistorPin, OUTPUT);
    pinMode(_greenLedPin, OUTPUT);
    pinMode(_redLedPin, OUTPUT);
    pinMode(_piezoPin, OUTPUT);
}

void ActuatorController::setSirenState(bool active)
{
    _isSirenActive = active;
    if (!_isSirenActive)
    {
        noTone(_piezoPin);
    }
}

void ActuatorController::update()
{
    updateSirenTone();
}

void ActuatorController::updateSirenTone()
{
    // If the siren is not active, exit immediately.
    if (!_isSirenActive)
    {
        return;
    }

    // Get the current time in milliseconds.
    unsigned long currentTime = millis();
    // Calculate how much time has passed since the last tone update.
    unsigned long timeElapsed = currentTime - _lastSirenUpdateTime;

    // Check if it's time to update the siren's tone.
    if (timeElapsed >= SIREN_UPDATE_INTERVAL_MS)
    {
        // Record the time of this update.
        _lastSirenUpdateTime = currentTime;

        // Play the current tone. This will continue until a new tone or noTone is called.
        tone(_piezoPin, _currentSirenFrequency);

        // Determine the next frequency by adjusting up or down.
        if (_isSirenSweepingUp)
        {
            // Increase the frequency for an upward sweep.
            _currentSirenFrequency += SIREN_FREQUENCY_STEP;

            // If the maximum frequency is reached, change direction.
            if (_currentSirenFrequency >= SIREN_MAX_FREQUENCY)
            {
                _isSirenSweepingUp = false;
            }
        }
        else
        {
            // Decrease the frequency for a downward sweep.
            _currentSirenFrequency -= SIREN_FREQUENCY_STEP;

            // If the minimum frequency is reached, change direction.
            if (_currentSirenFrequency <= SIREN_MIN_FREQUENCY)
            {
                _isSirenSweepingUp = true;
            }
        }
    }
}

void ActuatorController::setStatusGreen(bool active) { digitalWrite(_greenLedPin, active); }
void ActuatorController::setStatusRed(bool active) { digitalWrite(_redLedPin, active); }