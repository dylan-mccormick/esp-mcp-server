// serverDiscoverHandler.cpp

#include "mcp/jsonrpc/jsonRpcTypes.h"
#include "mcp/registries/mcpRequestRegistry.h"
#include "config/mcpConfig.h"

McpRequestHandlerResult serverDiscoverHandler(const JsonObjectConst req, JsonVariant result) {
    // Process supported versions
    JsonArray supportedVersions = result["supportedVersions"].to<JsonArray>();
    for (auto version : McpServerInfo::supportedProtocols) supportedVersions.add(version);

    // Server Info
    result["_meta"]["io.modelcontextprotocol/serverInfo"]["name"] = McpServerInfo::name;
    result["_meta"]["io.modelcontextprotocol/serverInfo"]["version"] = McpServerInfo::version;
    result["_meta"]["io.modelcontextprotocol/serverInfo"]["description"] = McpServerInfo::description;

    if (McpServerInfo::instructions) {
        result["instructions"] = McpServerInfo::instructions;
    }

    // Cache info
    result["ttlMs"] = McpServerInfo::ttlMs;
    result["cacheScope"] = "public";

    // Capabilities
    if (McpServerCapabilities::prompts) {
        result["capabilities"]["prompts"]["listChanged"] = McpServerCapabilities::promptsListChanged;
    }

    if (McpServerCapabilities::resources) {
        result["capabilities"]["resources"]["listChanged"] = McpServerCapabilities::resourcesListChanged;
        result["capabilities"]["resources"]["subscribe"] = McpServerCapabilities::resourcesSubscribe;
    }

    if (McpServerCapabilities::tools) {
        result["capabilities"]["tools"]["listChanged"] = McpServerCapabilities::toolsListChanged;
    }

    return McpRequestHandlerResult::success();
}

MCP_REQUEST_HANDLER("server/discover", serverDiscoverHandler);