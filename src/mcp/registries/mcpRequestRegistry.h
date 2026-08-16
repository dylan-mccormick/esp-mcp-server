// mcpRequestRegistry.h
// Registry that can be used to establish MCP request handlers based on their method

#pragma once
#include <ArduinoJson.h>

#include <functional>
#include <map>

#include "McpBaseRegistry.h"
#include "mcpRegistryUtils.h"

struct McpRequestHandlerResult {
    bool ok = true;
    int errorCode = 0;
    String errorMessage;

    static McpRequestHandlerResult success() {
        return {};
    };
    static McpRequestHandlerResult error(int code, const String& message) {
        return {false, code, message};
    };
};

using McpRequestHandlerFn = std::function<McpRequestHandlerResult(JsonObjectConst params, JsonVariant result)>;

class McpRequestRegistry : public McpBaseRegistry<McpRequestHandlerFn> {};

#define MCP_REQUEST_HANDLER(methodName, fn)                                     \
    static bool __attribute__((used)) MCP_HANDLER_CONCAT(_mcp_reg_, __LINE__) = \
        McpRequestRegistry::registerHandler(methodName, fn)
