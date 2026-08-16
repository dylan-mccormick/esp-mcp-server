// readResourceHandler.cpp
// Contains a handler for resources/read call

#include <ArduinoJson.h>

#include "mcp/handlers/tools/toolUtils.h"
#include "mcp/registries/mcpRequestRegistry.h"
#include "mcp/registries/mcpResourceRegistry.h"

McpRequestHandlerResult readResourceHandler(const JsonObjectConst params, JsonVariant result) {
    String uri;
    if (!requireArg(params, "uri", uri)) {
        return McpRequestHandlerResult::error(-32602, "uri must be provided as a string");
    }

    auto& handlers = McpResourceRegistry::handlers();
    auto target = handlers.find(uri);
    if (target == handlers.end()) {
        return McpRequestHandlerResult::error(-32602, String("Resource not found: ") + uri);
    }

    // execute handler
    target->second.handler(result);
    return McpRequestHandlerResult::success();
}

MCP_REQUEST_HANDLER("resources/read", readResourceHandler);