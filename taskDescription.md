# 🧪 Precision Fermentation Chamber Controller  
### *Object-Oriented Architecture & Predictive Logic*

---

## 🎯 General Objective

Develop a **complete firmware** for an **Arduino-based embedded system** that precisely manages a fermentation chamber. This project must showcase **advanced programming skills**, including:

- Implementation of **Object-Oriented Programming (OOP)**
- Handling real-world hardware challenges (e.g., **button debouncing**)
- Usage of **interrupts** for emergency events
- **Predictive control logic** to enhance system stability and efficiency

---

## 🛠️ Hardware Architecture *(to be built in Autodesk Tinkercad)*

The circuit must include the following components connected to an **Arduino Uno R3**:

| Component                   | Function                                                               |
|----------------------------|------------------------------------------------------------------------|
| **TMP36 Temperature Sensor** | Measures internal chamber temperature                                  |
| **MQ-3 Gas Sensor**          | Simulates ethanol detection (fermentation by-product)                  |
| **16x2 I2C LCD Display**     | Displays system status, sensor data, and configuration                |
| **Potentiometer**            | Allows the user to set the desired temperature (setpoint)             |
| **Navigation Button**        | Cycles through display screens                                        |
| **1-Channel Relay**          | Controls a heating element (simulated by a DC motor or resistor)      |
| **Bi-color LED**             | Provides visual feedback of operational status                        |
| **Emergency Stop Button**    | Instantly stops critical operations via external interrupt            |

---

## 🧩 Software Architecture & Functional Requirements

### 🧱 Object-Oriented Design (OOP)

The firmware must be structured using an **Object-Oriented Programming approach**. Responsibilities must be separated into `.h` and `.cpp` files. The following classes are required:

- `SensorManager`: Handles sensor readings
- `ActuatorController`: Manages relay and LED output
- `DisplayManager`: Controls LCD display content
- `DebouncedButton`: Reusable class that implements software debouncing using `millis()`
- `SystemState`: Manages the system’s main logic and finite state machine (FSM)

---

### 🔘 Debounced Input Management

- The **navigation button** must use **software debouncing** to ensure only one action per press.
- The logic must be implemented inside the `DebouncedButton` class using `millis()` timing.

---

### 🚨 Emergency Interrupt Handling

- The **emergency stop button** must be connected to **pin 2 or 3** (external interrupt pin).
- Pressing it should:
  - Trigger an **Interrupt Service Routine (ISR)**
  - Immediately halt critical operations (e.g., turn off the heater)
  - Transition the system to a **safe state**, overriding any current process

---

### 🔁 Finite State Machine (FSM)

The system must operate according to a well-defined **state machine**, including the following states:

| State             | Description                                          |
|------------------|------------------------------------------------------|
| `STANDBY`         | Idle state, awaiting user interaction                |
| `PREHEATING`      | Heats up to reach the user-defined temperature       |
| `MAINTAINING`     | Maintains temperature using predictive control logic |
| `EMERGENCY_STOP`  | Activated via interrupt; halts the system safely     |

---

### 🔮 Predictive Temperature Control Logic

In the `MAINTAINING` state, the controller must:

- Not wait for the temperature to drop below the threshold
- Analyze the **rate of temperature decrease** (first derivative over time)
- Activate the heater **proactively**, in short bursts
- Ensure **thermal stability** and reduce energy usage

---

### 📺 LCD User Interface

The LCD must clearly display:

- The current **operating state**
- The **real-time temperature**
- The **setpoint** value from the potentiometer
- System or warning messages such as:
  - `"Heating..."`
  - `"Temperature Stable"`
  - `"EMERGENCY STOP"`

---

## 🧪 Development & Testing Environment

- The entire system (circuit + firmware) must be designed, assembled, and tested **within Autodesk Tinkercad**.
- No physical components are required for this project.

---
