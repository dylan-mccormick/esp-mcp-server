// handlePinRead.cpp
// Contains a tool definition for the pinRead tool

#include <Arduino.h>
#include <ArduinoJson.h>
#include <unordered_set>
#include "mcp/registries/mcpToolRegistry.h"
#include "toolUtils.h"

static const std::unordered_set<uint8_t> PROHIBITED_PINS = {6, 7, 8, 9, 10, 11, 20, 24, 28, 29, 30, 31, 37, 38};

static const SchemaProperty pinReadProps[] = {
    { "pin", "integer", "gpio pin to read", true },
    { "mode", "string", "DIGITAL_PULLUP, DIGITAL_PULLDOWN, DIGITAL, or ANALOG", true }
};

static const ToolInputSchema pinReadSchema = { pinReadProps, 2 };

void handlePinRead(JsonObjectConst args, JsonVariant result) {
    // require pin
    uint8_t pin;
    if (!requireArg<uint8_t>(args, "pin", pin)) {
        writeToolError(result, "pin must be provided as an integer");
        return;
    }

    // require mode
    String mode;
    if (!requireArg<String>(args, "mode", mode)) {
        writeToolError(result, "mode must be provided as a string");
        return;
    }

    // pin valudation
    if (PROHIBITED_PINS.count(pin) > 0 || pin < 0 || pin > 39) {
        writeToolError(result, "pin must be between 0-39, must conform to the valid pins as described by the tool");
        return;
    }

    // mode validation
    uint8_t modeEnum;
    bool readAnalog = strcasecmp(mode.c_str(), "ANALOG") == 0;
    if (strcasecmp(mode.c_str(), "DIGITAL_PULLUP") == 0) modeEnum = INPUT_PULLUP;
    else if (strcasecmp(mode.c_str(), "DIGITAL_PULLDOWN") == 0) modeEnum = INPUT_PULLDOWN;
    else if (strcasecmp(mode.c_str(), "DIGITAL") == 0 || strcasecmp(mode.c_str(), "ANALOG") == 0) modeEnum = INPUT;

    if (std::unordered_set<uint8_t>{INPUT, INPUT_PULLUP, INPUT_PULLDOWN}.count(modeEnum) == 0) {
        writeToolError(result, "mode must be DIGITAL_PULLUP, DIGITAL_PULLDOWN, DIGITAL, or ANALOG");
        return;
    }

    if (modeEnum != INPUT && pin >= 34) { // these pins don't have the resistors to do PULLUP/PULLDOWN
        writeToolError(result, "gpio 34-39 cannot be used with DIGITAL_PULLUP or DIGITAL_PULLDOWN");
        return;
    }

    // read
    pinMode(pin, modeEnum);
    int readResult = readAnalog ? analogRead(pin) : digitalRead(pin);

    writeToolSuccess(result, String(readResult).c_str());
}

MCP_TOOL_DEF(
    "pinRead",
    "Reads the value of the requested pin. Set \"mode\" argument to DIGITAL_PULLUP to use INPUT_PULLUP that returns HIGH or LOW, DIGITAL_PULLDOWN to use INPUT_PULLDOWN that returns HIGH or LOW, DIGITAL for standard INPUT that returns HIGH OR LOW, ANALOG for standard INPUT that returns a value between 0-4095. GPIO 6-11, 20, 24, 28-31, 37, 38 are prohibited. Pins 34-39 may not be used with DIGITAL_PULLUP or DIGITAL_PULLDOWN. ANALOG should not be used with GPIOs 0, 2, 4, 12-15, and 25-27 as WiFi is enabled.",
    pinReadSchema,
    handlePinRead
);