// handlePinWriteSequence.cpp
// Contains a tool definition for the pinWriteSequence tool

#include <Arduino.h>
#include <ArduinoJson.h>

#include <unordered_set>

#include "mcp/registries/mcpToolRegistry.h"
#include "toolUtils.h"

static const std::unordered_set<uint8_t> PROHIBITED_PINS = {0,  1,  3,  6,  7,  8,  9,  10, 11, 20, 24,
                                                            28, 29, 30, 31, 34, 35, 36, 37, 38, 39};

void writePinWriteSequenceInputSchema(JsonObject inputSchema) {
    inputSchema["type"] = "object";
    JsonObject inputProperties = inputSchema["properties"].to<JsonObject>();
    JsonArray requiredProperties = inputSchema["required"].to<JsonArray>();
    requiredProperties.add("steps");

    JsonObject steps = inputProperties["steps"].to<JsonObject>();
    steps["type"] = "array";
    steps["description"] = "an ordered sequence of pin operations to execute";

    // schema for each passed step
    JsonObject stepItems = steps["items"].to<JsonObject>();
    stepItems["type"] = "object";

    JsonObject stepProperties = steps["properties"].to<JsonObject>();
    JsonArray stepRequired = steps["required"].to<JsonArray>();

    JsonObject pinProp = stepProperties["pin"].to<JsonObject>();
    pinProp["type"] = "integer";
    pinProp["description"] = "gpio pin to write to";
    stepRequired.add("pin");

    JsonObject analogValueProp = stepProperties["analogValue"].to<JsonObject>();
    analogValueProp["type"] = "integer";
    analogValueProp["description"] = "value between 0-4095 to be used as pwm output";

    JsonObject digitalValueProp = stepProperties["digitalValue"].to<JsonObject>();
    digitalValueProp["type"] = "string";
    digitalValueProp["description"] = "HIGH for on, LOW for off";

    JsonObject delayBeforeProp = stepProperties["delayBefore"].to<JsonObject>();
    delayBeforeProp["type"] = "integer";
    delayBeforeProp["description"] =
        "milliseconds to delay before running this operation; all successive operations will wait for this one to "
        "complete";
}

static const ToolInputSchema pinWriteSequenceSchema = {nullptr, 0, writePinWriteSequenceInputSchema};

void handlePinWriteSequence(const JsonObjectConst args, JsonVariant result) {}

MCP_TOOL_DEF(
    "pinWriteSequence",
    "Writes the specified value(s) to the requested pin(s). Can pass a 'delay' object to wait between pin writes. GPIO "
    "pins range between 0-39; GPIO 0, 1, 3, 6-11, 20, 24, 28-31, "
    "34-39 are prohibited. Must specify either an analogValue or a digitalValue, depending on whether to use PWM.",
    pinWriteSequenceSchema, handlePinWriteSequence);