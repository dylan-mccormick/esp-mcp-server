// handlePrintToSerial.cpp
// Contains a tool definition for the printToSerial tool

#include <ArduinoJson.h>
#include "mcp/registries/mcpToolRegistry.h"
#include "toolUtils.h"

static const SchemaProperty printToSerialProps[] = {
    { "message", "string", "message to print", true }
};

static const ToolInputSchema printToSerialSchema = { printToSerialProps, 1 };

void handlePrintToSerial(JsonObjectConst args, JsonVariant result) {
    String message;
    if (!requireArg<String>(args, "message", message)) {
        writeToolError(result, "message must be provided as a string");
        return;
    }

    if (!Serial) {
        writeToolError(result, "Serial is not set up");
        return;
    }

    Serial.print(message);
};

MCP_TOOL_DEF(
    "printToSerial",
    "Prints the specified message to the Serial stream.",
    printToSerialSchema,
    handlePrintToSerial
);