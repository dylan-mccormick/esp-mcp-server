// handlePinWrite.cpp
// Contains a tool definition for the pinWrite tool

#include "mcp/registries/mcpToolRegistry.h"
#include "toolUtils.h"
#include "pin_write/pinWriteUtils.h"

static const std::unordered_set<uint8_t> PROHIBITED_PINS = {0,  1,  3,  6,  7,  8,  9,  10, 11, 20, 24,
                                                            28, 29, 30, 31, 34, 35, 36, 37, 38, 39};

static const SchemaProperty pinWriteProps[] = {
    {"pin", "integer", "gpio pin to write to", true},
    {"analogValue", "integer", "value between 0-4095 to be used as pwm output", false},
    {"digitalValue", "string", "HIGH for on, LOW for off", false}};

static const ToolInputSchema pinWriteSchema = {pinWriteProps, 3};

void handlePinWrite(JsonObjectConst args, JsonVariant result) {
    const PinWrite::ValidationResult validation = PinWrite::validatePinArguments(args);
    if (!validation.ok) {
        writeToolError(result, validation.error);
    }

    PinWrite::handlePinOperations(validation);
    writeToolSuccess(result, "success");
};

MCP_TOOL_DEF(
    "pinWrite",
    "Writes the specified value to the requested pin. GPIO pins range between 0-39; GPIO 0, 1, 3, 6-11, 20, 24, 28-31, "
    "34-39 are prohibited. Must specify either an analogValue or a digitalValue, depending on whether to use PWM.",
    pinWriteSchema, handlePinWrite);