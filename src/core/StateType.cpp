#include "StateType.h"
#include <Arduino.h>


const String States::toString(States::Type state)
{
    switch (state)
    {
        case States::Type::STANDBY:         return "STANDBY";
        case States::Type::PREHEATING:      return "PREHEATING";
        case States::Type::MAINTAINING:     return "MAINTAINING";
        case States::Type::EMERGENCY_STOP:  return "EMERGENCY STOP";
        default:                            return "UNKNOWN";
    }
}