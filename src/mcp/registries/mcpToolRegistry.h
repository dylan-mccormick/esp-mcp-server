// mcpToolRegistry.h
// Registry for MCP tools

#pragma once
#include <ArduinoJson.h>

#include <functional>
#include <map>

#include "McpBaseRegistry.h"
#include "mcpRegistryUtils.h"

using McpToolExecFn = std::function<void(JsonObjectConst arguments, JsonVariant result)>;

struct SchemaProperty {
    const char* name;
    const char* type;
    const char* description;
    bool required;
};

struct ToolInputSchema {
    const SchemaProperty* properties;
    size_t propertyCount;
};

struct McpToolDef {
    const char* name;
    const char* description;
    ToolInputSchema schema;
    McpToolExecFn handler;
};

class McpToolRegistry : public McpBaseRegistry<McpToolDef> {};

#define MCP_TOOL_DEF(toolName, toolDesc, schema, handler)                       \
    static McpToolDef _tooldef_schema_ = {toolName, toolDesc, schema, handler}; \
    static bool __attribute__((used)) MCP_HANDLER_CONCAT(_mcp_reg_, __LINE__) = \
        McpToolRegistry::registerHandler(toolName, _tooldef_schema_)
