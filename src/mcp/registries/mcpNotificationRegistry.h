// mcpNotificationRegistry.h
// Registry that can be used to establish MCP notification handlers based on their method

#pragma once

#include <ArduinoJson.h>

#include <functional>
#include <map>

#include "mcpRegistryUtils.h"

using McpNotificationHandlerFn = std::function<void(JsonObjectConst params)>;

class McpNotificationRegistry : public McpBaseRegistry<McpNotificationHandlerFn> {};

#define MCP_NOTIFICATION_HANDLER(methodName, fn)                                 \
    static bool __attribute__((used)) MCP_HANDLER_CONCAT(methodName, __LINE__) = \
        McpNotificationRegistry::registerHandler(methodName, fn)
