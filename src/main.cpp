//=================================================================================
// main.cpp
// Main file (the "main" of the project)
// Responsibilities:
// - Include all custom libraries and classes.
// - Define hardware pins.
// - Create object instances.
// - Set the byteerrupt.
// - Run the main update loop.
// ============================================================================================


#include <Arduino.h>
#include "controllers/ActuatorController.h"
#include "sensors/SensorManager.h"
#include "input/DebouncedButton.h"
#include "display/DisplayManager.h"
#include "core/SystemState.h"

//  PIN AND COSTANT DEFINITIONS

// DIGITAL PINS DEFINITION
constexpr byte TRANSISTOR_PIN = 2; // Pin for the transistor which controls the motor
constexpr byte EMERGENCY_BUTTON_PIN = 3; // Pin for the emergency button
constexpr byte GREEN_LED_PIN = 10; // Pin for the green LED (indicates normal operation)
constexpr byte NAVIGATION_BUTTON_PIN = 11; // Pin for the navigation button
constexpr byte RED_LED_PIN = 12; // Pin for the red LED (indicates low level emergency)
constexpr byte PIEZO_PIN = 13; // Pin for the piezo buzzer (indicates high level emergency)

// ANALOG PINS DEFINITION
constexpr byte TEMPERATURE_SENSOR_PIN = A0; // Pin for the temperature sensor
constexpr byte GAS_SENSOR_PIN = A2; // Pin for the gas sensor
constexpr byte POTENTIOMETER_PIN = A3; // Pin for the potentiometer

// I2C ADDRESS
constexpr byte I2C_ADDRESS= 0x27; 

// OBJECT DEFINITIONS
ActuatorController actuatorController(TRANSISTOR_PIN, GREEN_LED_PIN, RED_LED_PIN, PIEZO_PIN);
SensorManager sensorManager(TEMPERATURE_SENSOR_PIN, GAS_SENSOR_PIN, POTENTIOMETER_PIN);
DebouncedButton navigationButton(NAVIGATION_BUTTON_PIN);
DisplayManager lcd(I2C_ADDRESS);
SystemState systemState(sensorManager, actuatorController, lcd);


/**
 * @brief This function is called by hardware when the emergency stop button is pressed.
 * It must be extremely fast. It delegates the work to the SystemState object.
 * The 'volatile' keyword is not strictly needed here as we are calling a method,
 * but it's good practice to be aware of it for ISRs that modify global flags.
 */
void emergencyStopISR() {
    systemState.triggerEmergencyStop();
}

void setup() {
  Serial.begin(9600);
  actuatorController.begin();
  sensorManager.begin();
  navigationButton.begin();
  lcd.begin();
  systemState.begin();
  pinMode(EMERGENCY_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON_PIN),  emergencyStopISR, FALLING);
}

void loop() {
  actuatorController.update();
  systemState.update();
}

