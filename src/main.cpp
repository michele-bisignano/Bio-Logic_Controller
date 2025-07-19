//=================================================================================
// main.cpp
// Main file (the "main" of the project)
// Responsibilities:
// - Include all custom libraries and classes.
// - Define hardware pins.
// - Create object instances.
// - Set the interrupt.
// - Run the main update loop.
// ============================================================================================


#include <Arduino.h>


//  PIN AND COSTANT DEFINITIONS

// DIGITAL PINS DEFINITION
constexpr int TRANSISTOR_PIN = 2; // Pin for the transistor which controls the motor
constexpr int EMERGENCY_BUTTON_PIN = 3; // Pin for the emergency button
constexpr int GREEN_LED_PIN = 10; // Pin for the green LED (indicates normal operation)
constexpr int NAVIGATION_BUTTON_PIN = 11; // Pin for the navigation button
constexpr int RED_LED_PIN = 12; // Pin for the red LED (indicates low level emergency)
constexpr int PIEZO_PIN = 13; // Pin for the piezo buzzer (indicates high level emergency)


// ANALOG PINS DEFINITION
constexpr int TEMPERATURE_SENSOR_PIN = A0; // Pin for the temperature sensor
constexpr int GAS_SENSOR_PIN = A2; // Pin for the gas sensor
constexpr int POTENTIOMETER_PIN = A3; // Pin for the potentiometer
constexpr int SDA_PIN = A4; // Pin for the I2C SDA line
constexpr int SCL_PIN = A5; // Pin for the I2C SCL line


// FUNCTION DEFINITIONS


void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}
