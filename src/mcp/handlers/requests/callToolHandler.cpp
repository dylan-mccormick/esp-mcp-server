// callToolHandler.cpp
// Contains a handler to deal with routing tool calls and returning responses

#include "mcp/registries/mcpRequestRegistry.h"
#include "mcp/registries/mcpToolRegistry.h"
#include "mcp/handlers/tools/toolUtils.h"

McpRequestHandlerResult callToolHandler(const JsonObjectConst params, JsonVariant result) {
    const char* toolName;
    if (!requireArg(params, "name", toolName)) {
        return McpRequestHandlerResult::error(-32602, "name must be provided as a string");
    }

    // try to find tool
    auto& tools = McpToolRegistry::handlers();
    auto target = tools.find(toolName);
    if (target == tools.end()) {
        return McpRequestHandlerResult::error(-32602, String("Unknown tool: ") + toolName);
    }

    // execute the tool
    target->second.handler(params["arguments"].as<JsonObjectConst>(), result);
    return McpRequestHandlerResult::success();
}

MCP_REQUEST_HANDLER("tools/call", callToolHandler);