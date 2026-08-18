// pinWriteUtils.cpp
// Contains implementations for the PinWrite namespace

#include "pinWriteUtils.h"

PinWrite::ValidationResult PinWrite::validatePinArguments(const JsonObjectConst args) {
    // require pin
    uint8_t pin;
    if (!requireArg<uint8_t>(args, "pin", pin)) {
        return { false, false, 0, false, false, "pin must be provided as an integer" };
    }

    // require value
    uint16_t analogValue;
    String digitalValue;
    bool hasAnalog = requireArg<uint16_t>(args, "analogValue", analogValue);
    bool hasDigital = requireArg<String>(args, "digitalValue", digitalValue);
    if (hasDigital == hasAnalog) {
        return { false, 0, false, 0, false, false, "either analogValue must be provided as an integer or digitalValue must be provided as a "
                       "string, but not both" };
    }

    // pin valudation
    if (PROHIBITED_PINS.count(pin) > 0 || pin < 0 || pin > 39) {
        return { false, 0, false, 0, false, false, "pin must be between 0-39, must conform to the valid pins as described by the tool" };
    }

    // analog validation
    if (hasAnalog && (analogValue < 0 || analogValue > 4095)) {
        return { false, 0, false, 0, false, false, "analogValue must range between 0-4095" };
    }

    // digital validation
    if (hasDigital && hasDigital &&
        !(strcasecmp("HIGH", digitalValue.c_str()) == 0 || strcasecmp("LOW", digitalValue.c_str()) == 0)) {
            return { false, 0, false, 0, false, false, "digitalValue must be either HIGH or LOW" };
        }

    return { true, pin, hasAnalog, analogValue, hasDigital, strcasecmp("HIGH", digitalValue.c_str()) == 0, nullptr };
}

void PinWrite::handlePinOperations(const PinWrite::ValidationResult& info) {
    if (info.hasAnalog) {
        pinMode(info.pin, OUTPUT);
        analogWrite(info.pin, info.analogValue);
        return;
    }

    pinMode(info.pin, OUTPUT);
    digitalWrite(info.pin, info.digitalValue ? HIGH : LOW);
}