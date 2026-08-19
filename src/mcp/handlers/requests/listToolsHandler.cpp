// listToolsHandler.cpp
// Contains a handler for a tools/list call

#include "mcp/registries/mcpRequestRegistry.h"
#include "mcp/registries/mcpToolRegistry.h"

McpRequestHandlerResult listToolsHandler(const JsonObjectConst req, JsonVariant result) {
    JsonArray toolsArr = result["tools"].to<JsonArray>();
    for (const auto& [_, schema] : McpToolRegistry::handlers()) {
        JsonObject data = toolsArr.add<JsonObject>();

        // write tool data
        data["name"] = schema.name;
        data["description"] = schema.description;
        JsonObject inputSchema = data["inputSchema"].to<JsonObject>();

        inputSchema["type"] = "object";
        JsonObject inputProperties = inputSchema["properties"].to<JsonObject>();
        JsonArray requiredProperties = inputSchema["required"].to<JsonArray>();

        // parse each schema object
        for (size_t i = 0; i < schema.schema.propertyCount; i++) {
            SchemaProperty target = schema.schema.properties[i];
            JsonObject property = inputProperties[target.name].to<JsonObject>();

            property["type"] = target.type;
            property["description"] = target.description;

            if (target.required) requiredProperties.add(target.name);
        }
    }

    // cache info
    result["ttlMs"] = 300000;
    result["cacheScope"] = "public";

    return McpRequestHandlerResult::success();
}

MCP_REQUEST_HANDLER("tools/list", listToolsHandler);