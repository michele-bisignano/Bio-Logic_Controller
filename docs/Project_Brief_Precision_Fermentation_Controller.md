# Project Brief: Precision Fermentation Chamber Controller

## 1. General Objective

The objective is to develop robust and complete firmware for an Arduino-based embedded system capable of precisely managing the environment within a fermentation chamber. This project must demonstrate advanced software engineering skills through the mandatory implementation of an **Object-Oriented Programming (OOP)** architecture. Key features include managing real-world hardware events, using interrupts for critical safety operations, and implementing predictive control logic to optimize system performance and stability.

## 2. Hardware Architecture (Simulated in Tinkercad)

The circuit must be composed of the following components, connected to an Arduino Uno R3:

*   **Temperature Sensor (TMP36):** To accurately measure the internal temperature of the chamber.
*   **Gas Sensor (MQ-3):** To simulate the monitoring of a fermentation by-product, such as ethanol.
*   **16x2 LCD Display (with I2C module):** To provide a real-time, flicker-free display of system status, sensor data, and settings.
*   **Potentiometer:** To allow the user to dynamically set the desired temperature (setpoint).
*   **1-Channel Relay:** Connected to a heating element (simulated by a DC motor or power resistor) to actively control the temperature.
*   **Bi-color LED (or one Red and one Green LED):** To provide immediate visual indication of the system's operational status.
*   **Piezo Buzzer:** To act as an audible siren during a high-level emergency.
*   **Emergency Stop Button:** A dedicated hardware button for handling critical safety events.

## 3. Software Architecture and Functional Requirements

### 3.1. Object-Oriented (OOP) Structure
The firmware must be designed following a strict Object-Oriented architecture, separating responsibilities into distinct classes. At a minimum, the following classes must be created:
*   `SensorManager`: For handling all sensor readings (temperature, gas, potentiometer).
*   `ActuatorController`: For controlling all output devices (heater relay, LEDs, siren).
*   `DisplayManager`: For all interactions with the LCD screen.
*   `SystemState`: The core class containing the main application logic and the state machine.

### 3.2. Control Logic and Finite State Machine (FSM)
The system must operate according to a well-defined state machine, which includes at least the following states:
*   **STANDBY:** The system is idle, waiting for the temperature to drop below the setpoint. Actuators are off.
*   **PREHEATING:** The system is actively heating to reach the setpoint. An indicator light must show this active state.
*   **MAINTAINING:** The system has reached the setpoint and is now working to maintain it within a stable range. A "stable" indicator light must be active.
*   **EMERGENCY_STOP:** A terminal state activated by a hardware interrupt.

### 3.3. Predictive Control Logic
During the `MAINTAINING` state, the system must not simply reactivate the heater when the temperature drops below a threshold. Instead, it must analyze the **rate of temperature change (its derivative over time)** to proactively activate the heater with short, calculated pulses. This anticipates heat loss, ensuring greater thermal stability and lower energy consumption.

### 3.4. Emergency and Safety Handling
The system must manage two distinct types of emergencies:

*   **Software Emergency (High Gas Level):** If the gas sensor detects a critically high level of by-products, the system must:
    1.  Immediately deactivate the heating element.
    2.  Activate a visual alarm (Red LED) and an audible alarm (Siren).
    3.  Display a "GAS WARNING!" message on the LCD.
    4.  **Automatically recover:** If the gas level returns to a safe value, the system must automatically disable the alarms and resume its previous operational state (e.g., return to `MAINTAINING` if it was interrupted from there).

*   **Hardware Emergency (Emergency Stop Button):**
    1.  The Emergency Stop button must be connected to an **external interrupt pin** (pin 2 or 3).
    2.  Pressing it must trigger an Interrupt Service Routine (ISR) that immediately puts the system into the `EMERGENCY_STOP` state.
    3.  In this state, all actuators must be deactivated (except for permanent visual and audible alarms), and a "HW STOP ACTIVATED" message must be displayed.
    4.  **This state is terminal.** The system is considered to be in an unrecoverable condition that requires manual intervention. It **must not** exit this state automatically; a physical reset of the Arduino is required to restart the system.

### 3.5. User Interface on LCD
The LCD display must provide clear, non-flickering visual feedback, showing the current state, current temperature, setpoint, gas value, and relevant status or alarm messages.
