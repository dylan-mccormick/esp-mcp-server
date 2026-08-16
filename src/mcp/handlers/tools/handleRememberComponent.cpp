// handleRememberComponent.cpp
// Contains a tool definition for the rememberComponent tool

#include "mcp/registries/mcpToolRegistry.h"
#include "mcp/handlers/resources/componentMemory.h"
#include "toolUtils.h"

static const SchemaProperty rememberComponentProps[] = {
    { "pin", "integer", "gpio pin to remember", true },
    { "name", "string", "short identifier for the wire connected to this pin" },
    { "notes", "string", "detailed description of the wire connected to this pin and its specific purpose" }
};

static const ToolInputSchema rememberComponentSchema = { rememberComponentProps, 3 };

void handleRememberComponent(JsonObjectConst args, JsonVariant result) {
    // require pin
    uint8_t pin;
    if (!requireArg<uint8_t>(args, "pin", pin)) {
        writeToolError(result, "pin must be provided as an integer");
        return;
    }

    // require name
    const char* name;
    if (!requireArg<const char*>(args, "name", name)) {
        writeToolError(result, "name must be provided as a string");
        return;
    }

    // require notes
    const char* notes;
    if (!requireArg<const char*>(args, "notes", notes)) {
        writeToolError(result, "notes must be provided as a string");
        return;
    }

    ComponentMemory::remember(pin, name, notes);
    writeToolSuccess(result, "success");
}

MCP_TOOL_DEF(
    "rememberComponent",
    "Saves a name/description to a specific pin. In other words, updates the esp32://components resource to reflect any component that has been newly connected to the ESP32.",
    rememberComponentSchema,
    handleRememberComponent
);