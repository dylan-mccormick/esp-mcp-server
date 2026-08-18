// handlePinWriteSequence.cpp
// Contains a tool definition for the pinWriteSequence tool

#include <Arduino.h>
#include <ArduinoJson.h>

#include <unordered_set>

#include "mcp/registries/mcpToolRegistry.h"
#include "toolUtils.h"

static const std::unordered_set<uint8_t> PROHIBITED_PINS = {0,  1,  3,  6,  7,  8,  9,  10, 11, 20, 24,
                                                            28, 29, 30, 31, 34, 35, 36, 37, 38, 39};

static const SchemaProperty stepsEntryProps[] = {
    {"pin", "integer", "gpio pin to write to", true},
    {"analogValue", "integer", "value between 0-4095 to be used as pwm output", false},
    {"digitalValue", "string", "HIGH for on, LOW for off", false},
    {"delayBefore", "integer",
     "milliseconds to delay before running this operation; all successive operations will wait for this one to "
     "complete",
     false}};

static const ToolInputSchema stepsEntrySchema[] = {stepsEntryProps, 1};

static const SchemaProperty pinWriteSequenceProps[] = {
    {"steps", "array", "An array of steps to take during the pin write sequence.", true}};

void writePinWriteSequenceInputSchema(JsonObject inputSchema) {}

static const ToolInputSchema pinWriteSequenceSchema = {nullptr, 0, writePinWriteSequenceInputSchema};

void handlePinWriteSequence(const JsonObjectConst args, JsonVariant result) {}

MCP_TOOL_DEF(
    "pinWriteSequence",
    "Writes the specified value(s) to the requested pin(s). Can pass a 'delay' object to wait between pin writes. GPIO "
    "pins range between 0-39; GPIO 0, 1, 3, 6-11, 20, 24, 28-31, "
    "34-39 are prohibited. Must specify either an analogValue or a digitalValue, depending on whether to use PWM.",
    pinWriteSequenceSchema, handlePinWriteSequence);