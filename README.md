<div align="center">
  <h1>Bio-Logic Controller</h1>
  <p>
    <strong>A Precision Fermentation Chamber Controller with a Predictive, Object-Oriented Core.</strong>
  </p>
  <a href="https://www.tinkercad.com/things/iKqZgCNgBgO-bio-logic-controller">
    <img src="https://img.shields.io/badge/Tinkercad-Live%20Simulation-F96434?style=for-the-badge&logo=tinkercad" alt="Live Simulation on Tinkercad"/>
  </a>
</div>

---

> **This project is a deep-dive into embedded systems engineering, demonstrating how to build not just a functional device, but a robust, intelligent, and scalable system.**

For a detailed breakdown of the project's requirements and constraints, please see the [**`taskDescription.md`**](./taskDescription.md) file.

## 🔬 What is Bio-Logic Controller?

The **Bio-Logic Controller** is a sophisticated embedded system designed to precisely manage the environment inside a fermentation chamber. Biological processes like fermentation are extremely sensitive to environmental changes. This system ensures the ideal conditions are not only met but are maintained with stability and efficiency.

The core task is to solve the problem of environmental instability. Instead of a simple thermostat that reacts to temperature changes, this controller **anticipates** them, creating a perfectly stable environment crucial for high-quality results in applications like:
*   Artisanal Food Production (yogurt, beer, sourdough)
*   Bio-Hacking and Scientific Experiments
*   Small-scale Pharmaceutical Cultures

---

## ✨ Key Features

*   🧠 **Predictive Control Logic:** Uses the rate of temperature change to proactively manage the heating element, preventing overshoots and undershoots.
*   🧱 **Object-Oriented Architecture:** Fully modular code written in C++ with distinct classes for sensors, actuators, and logic, making it clean, scalable, and easy to maintain.
*   ⚡ **Interrupt-Driven Safety:** An emergency stop button is handled by a hardware interrupt, guaranteeing an instant and reliable system shutdown.
*   📟 **Real-Time Monitoring:** An LCD interface provides immediate feedback on system status, current temperature, and user-set parameters.
*   ⚙️ **Robust Input Handling:** Implements software debouncing to ensure user inputs are precise and error-free.

---

## ⚙️ How It Works

The system's intelligence is built upon a combination of a Finite State Machine (FSM) and predictive algorithms.

### The Core Logic: Finite State Machine (FSM)
The controller operates based on a well-defined FSM to ensure it's always in a predictable state:
1.  **`STANDBY`**: The system is idle, waiting for a target temperature to be set.
2.  **`PREHEATING`**: The heater is fully engaged to bring the chamber to the desired temperature as quickly as possible.
3.  **`MAINTAINING`**: The core operational state. The system uses its predictive logic to keep the temperature stable.
4.  **`EMERGENCY_STOP`**: A critical, interrupt-triggered state that immediately shuts down all actuators.

### The "Wow" Factor: Predictive Control
In the `MAINTAINING` state, a simple controller would turn the heater on only *after* the temperature drops below the target. The **Bio-Logic Controller** is smarter:
> It constantly calculates the **rate of temperature change (the derivative)**. If it detects that the chamber is cooling down too quickly, it applies short, corrective pulses to the heater *before* the temperature actually drops below the setpoint. This proactive approach results in an incredibly stable thermal environment.

---

## 🛠️ Development & Testing Process

This project was developed following a structured, phased approach to ensure robustness and code quality.

#### **Phase 1: Scaffolding and Hardware Abstraction**
The first step was to build the Object-Oriented foundation. I created the class skeletons (`.h` and `.cpp` files) for `SensorManager`, `ActuatorController`, `DisplayManager`, and `DebouncedButton`. Each class was implemented and tested in isolation to ensure the hardware components could be reliably controlled.

#### **Phase 2: State Machine and User Input**
With the hardware abstracted, I implemented the core `SystemState` class, which contains the FSM. The logic for transitioning between `STANDBY`, `PREHEATING`, and `MAINTAINING` was developed. In parallel, the `DebouncedButton` class was perfected to provide a reliable, single-press-per-click user input mechanism.

#### **Phase 3: Advanced Logic and Safety Implementation**
This phase focused on the project's most complex features.
*   The **predictive control algorithm** was coded and integrated into the `MAINTAINING` state.
*   The **emergency stop button** was configured to use a hardware interrupt (`attachInterrupt()`), and the Interrupt Service Routine (ISR) was written to be as fast and efficient as possible.

#### **Phase 4: Integration Testing on Tinkercad**
The entire system was tested within the Tinkercad simulation environment. I ran multiple scenarios to validate its behavior:
*   **Nominal Operation:** Setting a temperature and observing the system's ability to reach and maintain it.
*   **Stress Test:** Rapidly changing the simulated ambient temperature to see how the predictive logic compensated.
*   **User Interaction Test:** Cycling through menus and changing the setpoint to test UI responsiveness.
*   **Safety Test:** Triggering the emergency stop interrupt during every possible state to confirm its immediate and overriding effect.

---

## 🚀 Try It Yourself!

You can run and interact with a live simulation of the project directly on Tinkercad. Click the link below to see the Bio-Logic Controller in action!

<div align="center">
  <a href="https://www.tinkercad.com/things/iKqZgCNgBgO-bio-logic-controller">
    <h2>▶️ Launch Live Simulation</h2>
  </a>
</div>
