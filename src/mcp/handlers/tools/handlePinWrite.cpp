// handlePinWrite.cpp
// Contains a tool definition for the pinWrite tool

#include <Arduino.h>
#include <ArduinoJson.h>

#include <unordered_set>

#include "mcp/registries/mcpToolRegistry.h"
#include "toolUtils.h"

static const std::unordered_set<uint8_t> PROHIBITED_PINS = {0,  1,  3,  6,  7,  8,  9,  10, 11, 20, 24,
                                                            28, 29, 30, 31, 34, 35, 36, 37, 38, 39};

static const SchemaProperty pinWriteProps[] = {
    {"pin", "integer", "gpio pin to write to", true},
    {"analogValue", "integer", "value between 0-4095 to be used as pwm output", false},
    {"digitalValue", "string", "HIGH for on, LOW for off", false}};

static const ToolInputSchema pinWriteSchema = {pinWriteProps, 3};

void handlePinWrite(JsonObjectConst args, JsonVariant result) {
    // require pin
    uint8_t pin;
    if (!requireArg<uint8_t>(args, "pin", pin)) {
        writeToolError(result, "pin must be provided as an integer");
        return;
    }

    // require value
    uint16_t analogValue;
    String digitalValue;
    bool hasAnalog = requireArg<uint16_t>(args, "analogValue", analogValue);
    bool hasDigital = requireArg<String>(args, "digitalValue", digitalValue);
    if (hasDigital == hasAnalog) {
        writeToolError(result,
                       "either analogValue must be provided as an integer or digitalValue must be provided as a "
                       "string, but not both");
        return;
    }

    // pin valudation
    if (PROHIBITED_PINS.count(pin) > 0 || pin < 0 || pin > 39) {
        writeToolError(result, "pin must be between 0-39, must conform to the valid pins as described by the tool");
        return;
    }

    // analog processing
    if (hasAnalog && (analogValue < 0 || analogValue > 4095)) {
        writeToolError(result, "analogValue must range between 0-4095");
        return;
    } else if (hasAnalog) {
        pinMode(pin, OUTPUT);
        analogWrite(pin, analogValue);
    }

    // digital processing
    if (hasDigital &&
        !(strcasecmp("HIGH", digitalValue.c_str()) == 0 || strcasecmp("LOW", digitalValue.c_str()) == 0)) {
        writeToolError(result, "digitalValue must be either HIGH or LOW");
        return;
    } else if (hasDigital) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, strcasecmp("HIGH", digitalValue.c_str()) == 0 ? HIGH : LOW);
    }

    writeToolSuccess(result, "success");
};

MCP_TOOL_DEF(
    "pinWrite",
    "Writes the specified value to the requested pin. GPIO pins range between 0-39; GPIO 0, 1, 3, 6-11, 20, 24, 28-31, "
    "34-39 are prohibited. Must specify either an analogValue or a digitalValue, depending on whether to use PWM.",
    pinWriteSchema, handlePinWrite);