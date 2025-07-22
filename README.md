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

> **This project is a deep-dive into embedded systems engineering, demonstrating how to build not just a functional device, but a robust, intelligent, and scalable system through a software-first mindset.**

### Project Documentation
*   [**Project Brief**](./docs/Project_Brief_Precision_Fermentation_Controller.md): The original project requirements and constraints.
*   [**Solution Retrospective**](./docs/Solution_Retrospective_OOP_Firmware_Design.md): A detailed analysis of the final architecture and design philosophy.

---

## 🔬 What is Bio-Logic Controller?

The **Bio-Logic Controller** is a sophisticated embedded system designed to precisely manage the environment inside a fermentation chamber. Biological processes like fermentation are extremely sensitive to environmental changes. This system ensures the ideal conditions are not only met but are maintained with stability and efficiency.

The core task is to solve the problem of environmental instability. Instead of a simple thermostat that reacts to temperature changes, this controller **anticipates** them, creating a perfectly stable environment crucial for high-quality results in applications like:
*   Artisanal Food Production (yogurt, beer, sourdough)
*   Bio-Hacking and Scientific Experiments
*   Small-scale Pharmaceutical Cultures

---

## ✨ Key Features

*   🧠 **Predictive Control Logic:** Uses the rate of temperature change to proactively manage the heating element, preventing overshoots and undershoots for maximum stability.
*   🧱 **Object-Oriented Architecture:** Fully modular C++ code with distinct classes for sensors, actuators, and logic, making the system clean, scalable, and easy to maintain.
*   ⚡ **Interrupt-Driven Safety:** A hardware emergency stop button guarantees an instant and reliable system shutdown, handled by a processor interrupt for maximum priority.
*   ⚙️ **Automatic Emergency Recovery:** The system intelligently detects and responds to high gas levels, and automatically returns to its previous operational state once conditions are safe.
*   📟 **Real-Time Monitoring:** A flicker-free LCD interface provides immediate feedback on system status, current temperature, setpoint, and active alarms.

---

## ⚙️ How It Works

The system's intelligence is built upon a combination of a Finite State Machine (FSM), predictive algorithms, and a clear hierarchy for safety logic.

### 1. The Core Logic: Finite State Machine (FSM)
The controller operates based on a well-defined FSM to ensure it's always in a predictable state:
*   **`STANDBY`**: The system is idle. The heater and all status LEDs are off.
*   **`PREHEATING`**: The heater is fully engaged, and a red LED indicates the system is actively working to reach the target temperature.
*   **`MAINTAINING`**: The core operational state. A green LED indicates the target temperature has been reached, and the predictive logic is now active to keep it stable.
*   **`EMERGENCY_STOP`**: A critical, non-recoverable state triggered by the hardware button.

### 2. The "Wow" Factor: Predictive Control
In the `MAINTAINING` state, a simple controller would turn the heater on only *after* the temperature drops. The **Bio-Logic Controller** is smarter:
> It constantly calculates the **rate of temperature change (the derivative)**. If it detects that the chamber is cooling down too quickly, it applies short, corrective pulses to the heater *before* the temperature actually drops below the setpoint. This proactive approach results in an incredibly stable thermal environment.

### 3. Safety and Emergency Logic
The system has a clear priority for handling emergencies:
1.  **Hardware Emergency (Highest Priority):** Pressing the **Emergency Stop Button** triggers a hardware interrupt. This immediately forces the system into the `EMERGENCY_STOP` state, from which it cannot recover without a physical reset. This ensures ultimate safety.
2.  **Software Emergency (High Gas Level):** If the gas sensor detects a critical level, the system enters an override mode:
    *   It immediately saves its current state (e.g., `MAINTAINING`).
    *   It deactivates the heater and activates the red LED and siren.
    *   Once the gas level returns to normal, it automatically disables the alarms and **restores its previous state**, seamlessly resuming its task.

---

## 🏛️ Final Architecture & Design Philosophy

This project was built with a **software-first** philosophy. Instead of adding hardware to solve problems, challenges were met with intelligent code and robust architecture. The final design is a testament to the power of Object-Oriented Programming in creating clean and maintainable embedded systems.

The responsibilities are cleanly separated into four main classes: `SensorManager`, `ActuatorController`, `DisplayManager`, and the core `SystemState` class, which acts as the brain of the operation. This modularity means the system can be easily adapted or scaled in the future. For a deeper analysis of the architecture, see the [**Solution Retrospective**](./docs/Solution_Retrospective_OOP_Firmware_Design.md).

---

## 🚀 Try It Yourself!

You can run and interact with a live simulation of the project directly on Tinkercad. Click the link below to see the Bio-Logic Controller in action!

<div align="center">

## [▶️ Launch Live Simulation](https://www.tinkercad.com/things/iKqZgCNgBgO-bio-logic-controller)

</div>
