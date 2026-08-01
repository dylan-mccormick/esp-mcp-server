// mcpRequestRegistry.h
// Registry that can be used to establish MCP request handlers based on their method

#pragma once
#include <functional>
#include <map>
#include <ArduinoJson.h>
#include "mcpRegistryUtils.h"
#include "McpBaseRegistry.h"

using McpRequestHandlerFn = std::function<void(JsonObjectConst params, JsonVariant result)>;

class McpRequestRegistry : public McpBaseRegistry<McpRequestHandlerFn> {};

#define MCP_REQUEST_HANDLER(methodName, fn) \
    static bool __attribute__((used)) MCP_HANDLER_CONCAT(_mcp_reg_, __LINE__) = McpRequestRegistry::registerHandler(methodName, fn)
