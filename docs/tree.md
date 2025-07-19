'''
src/                                # Contains all source code
├── main.cpp # Main entry point, coordinates all
├── core/                           # Contains the core logic
│   ├── SystemState.h             # Defines the state machine
│   └── SystemState.cpp           # Implements the state logic
├── controllers/                    # Manages actuators (outputs)
│   ├── ActuatorController.h      # Defines actuator control
│   └── ActuatorController.cpp    # Implements actuator control
├── display/                        # Manages the user interface
│   ├── DisplayManager.h          # Defines display management
│   └── DisplayManager.cpp        # Implements display management
├── input/                          # Manages user inputs
│   ├── DebouncedButton.h         # Defines the debounced button
│   └── DebouncedButton.cpp       # Implements the debounce logic
└── sensors/                        # Manages sensor readings
    ├── SensorManager.h           # Defines sensor management
    └── SensorManager.cpp         # Implements sensor management
'''