#pragma once

#include <Arduino.h>

/**
 * @file StateType.h
 * @brief Defines the system's operational states and related utilities within a dedicated namespace.
 */
namespace States
{

    /**
     * @enum Type
     * @brief Represents the discrete operational states of the Finite State Machine (FSM).
     *
     * Using a strongly-typed 'enum class' prevents name collisions and implicit conversions
     * to integers, thereby increasing type safety and code robustness.
     */
    enum class Type
    {
        /**
         * @brief The idle state. The system is powered on but is not performing any active control.
         * It monitors sensors and waits for conditions to start the process (e.g., setpoint > temp).
         * All primary actuators are deactivated.
         */
        STANDBY,

        /**
         * @brief The preheating state. The heater is continuously activated to bring the fermentation
         * chamber to the target setpoint temperature as quickly as possible.
         */
        PREHEATING,

        /**
         * @brief The maintaining state. This is the main operational state where the system uses
         * predictive control logic to keep the temperature stable around the setpoint, minimizing
         * oscillations and energy consumption.
         */
        MAINTAINING,

        /**
         * @brief The emergency stop state. This is a terminal state triggered by an external
         * interrupt (the emergency button). In this state, all critical actuators are immediately
         * deactivated, and the system remains locked until a manual reset.
         */
        EMERGENCY_STOP
    };

    /**
     * @brief Converts a States::Type enum value to its string representation.
     * @param state The state to convert.
     * @return A const String pointing to the string literal in read-only memory.
     */
    const String toString(Type state){
    switch (state) {
        case States::Type::STANDBY:         return "STANDBY";
        case States::Type::PREHEATING:      return "PREHEATING";
        case States::Type::MAINTAINING:     return "MAINTAINING";
        case States::Type::EMERGENCY_STOP:  return "EMERGENCY STOP";
        default:                              throw "UNKNOWN";
    }
}

}