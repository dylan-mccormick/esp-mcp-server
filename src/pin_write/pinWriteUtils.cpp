// pinWriteUtils.cpp
// Contains implementations for the PinWrite namespace

#include "pinWriteUtils.h"

#include "mcp/handlers/tools/toolUtils.h"
#include "pinWriteTypes.h"

PinWrite::ValidationResult PinWrite::ValidationResult::errorResult(const char* message) {
    return {false, message, {0, 0, false, 0}};
}

PinWrite::ValidationResult PinWrite::validatePinArguments(const JsonObjectConst args) {
    // require pin
    int pinValue;
    if (!requireArg<int>(args, "pin", pinValue) || pinValue < 0 || pinValue > 39) {
        return PinWrite::ValidationResult::errorResult("pin must be provided as an integer");
    }
    const uint8_t pin = static_cast<uint8_t>(pinValue);

    // require value
    int analogValue;
    String digitalValue;
    bool hasAnalog = requireArg<int>(args, "analogValue", analogValue);
    bool hasDigital = requireArg<String>(args, "digitalValue", digitalValue);
    if (hasDigital == hasAnalog) {
        return PinWrite::ValidationResult::errorResult(
            "either analogValue must be provided as an integer or digitalValue must be provided as a string, but not "
            "both");
    }

    // optional delaAfter
    int delayAfter = 0;
    if (!args["delayAfter"].isNull() && (!requireArg<int>(args, "delayAfter", delayAfter) || delayAfter < 0)) {
        return PinWrite::ValidationResult::errorResult("delayAfter must be a non-negative integer");
    }

    // pin valudation
    if (PROHIBITED_PINS.count(pin) > 0 || pin > 39) {
        return PinWrite::ValidationResult::errorResult(
            "pin must be between 0-39, must conform to the valid pins as described by the tool");
    }

    // analog validation
    if (hasAnalog && (analogValue < 0 || analogValue > 4095)) {
        return PinWrite::ValidationResult::errorResult("analogValue must range between 0-4095");
    }

    // digital validation
    if (hasDigital && hasDigital &&
        !(strcasecmp("HIGH", digitalValue.c_str()) == 0 || strcasecmp("LOW", digitalValue.c_str()) == 0)) {
        return PinWrite::ValidationResult::errorResult("digitalValue must be either HIGH or LOW");
    }

    return {true,
            nullptr,
            {pin, delayAfter, hasDigital,
             static_cast<uint16_t>(hasDigital ? (strcasecmp("HIGH", digitalValue.c_str()) == 0 ? HIGH : LOW)
                                              : analogValue)}};
}

void PinWrite::handlePinOperations(const PinWrite::PinOperation& info) {
    pinMode(info.pin, OUTPUT);

    if (!info.digital) {
        analogWrite(info.pin, info.value);
        return;
    }

    digitalWrite(info.pin, info.value);
}