// listResourcesHandler.cpp
// Contains a handler for resources/list call

#include "mcp/registries/mcpRequestRegistry.h"
#include "mcp/registries/mcpResourceRegistry.h"

McpRequestHandlerResult listResourcesHandler(const JsonObjectConst args, JsonVariant result) {
    JsonArray resourcesArr = result["resources"].to<JsonArray>();
    for (const auto& [_, details] : McpResourceRegistry::handlers()) {
        JsonObject data = resourcesArr.add<JsonObject>();

        // write resource data
        data["uri"] = details.uri;
        data["name"] = details.name;
        data["description"] = details.description;
        data["mimeType"] = details.mimeType;
    }

    // cache data
    result["ttlMs"] = 300000;
    result["cacheScope"] = "public";

    return McpRequestHandlerResult::success();
}

MCP_REQUEST_HANDLER("resources/list", listResourcesHandler);