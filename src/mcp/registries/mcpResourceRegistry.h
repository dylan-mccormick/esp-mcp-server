// mcpResourceRegistry.h
// Registry that can be used to serve resource handlers at specific URIs

#pragma once

#include <ArduinoJson.h>

#include <functional>

#include "McpBaseRegistry.h"
#include "mcpRegistryUtils.h"

using McpResourceHandlerFn = std::function<void(JsonVariant result)>;

struct McpResourceDef {
    const char* uri;
    const char* name;
    const char* description;
    const char* mimeType;
    McpResourceHandlerFn handler;
};

class McpResourceRegistry : public McpBaseRegistry<McpResourceDef> {};

#define MCP_RESOURCE_DEF(uri, name, description, mimeType, handler)                  \
    static McpResourceDef _resource_def_{uri, name, description, mimeType, handler}; \
    static bool __attribute__((used)) MCP_HANDLER_CONCAT(_mcp_resource_, __LINE__) = \
        McpResourceRegistry::registerHandler(uri, _resource_def_)
