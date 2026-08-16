// handleForgetComponent.cpp
// Contains a tool definition for the forgetComponent tool

#include "mcp/handlers/resources/componentMemory.h"
#include "mcp/registries/mcpToolRegistry.h"
#include "toolUtils.h"

static const SchemaProperty forgetComponentProps[] = {{"pin", "integer", "gpio pin to forget", true}};

static const ToolInputSchema forgetComponentSchema = {forgetComponentProps, 1};

void handleForgetComponent(JsonObjectConst args, JsonVariant result) {
    // require pin
    uint8_t pin;
    if (!requireArg<uint8_t>(args, "pin", pin)) {
        writeToolError(result, "pin must be provided as an integer");
        return;
    }

    ComponentMemory::forget(pin);
    writeToolSuccess(result, "success");
}

MCP_TOOL_DEF("forgetComponent",
             "Removes a name/notes description pair for the specified pin. This will update the esp32://components "
             "resource to forget what is connected to the specified pin.",
             forgetComponentSchema, handleForgetComponent);