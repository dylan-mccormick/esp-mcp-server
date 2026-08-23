#include <ESPAsyncWebServer.h>

#include <vector>

#include "config/mcpConfig.h"
#include "mcp/jsonrpc/jsonRpcCodec.h"
#include "mcp/mcpDispatch.h"
#include "mcp/registries/mcpRequestRegistry.h"

#ifndef MCP_MAX_BODY_SIZE
#define MCP_MAX_BODY_SIZE 8192
#endif

struct MCPBodyBuffer {
    std::vector<uint8_t> data;
};

ArBodyHandlerFunction mcpHandler = [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index,
                                      size_t total) {
    // First chunk: alloc buffer to handle data
    if (index == 0) {
        if (total == 0 || total > MCP_MAX_BODY_SIZE) {
            request->send(413, "text/plain", "Request body too large");
            return;
        }
        auto* buffer = new MCPBodyBuffer();
        buffer->data.reserve(total);
        request->_tempObject = buffer;
    }

    // Insert data into buffer
    auto* buffer = static_cast<MCPBodyBuffer*>(request->_tempObject);
    if (!buffer || index + len > total || buffer->data.size() + len > MCP_MAX_BODY_SIZE) {
        delete buffer;
        request->_tempObject = nullptr;
        request->send(413, "text/plain", "Request body too large");
        return;
    }
    buffer->data.insert(buffer->data.end(), data, data + len);

    // Check if there are more chunks
    if (index + len < total) return;

    // Process full body
    JsonDocument reqDoc;
    DeserializationError err = deserializeJson(reqDoc, buffer->data.data(), buffer->data.size());

    delete buffer;
    request->_tempObject = nullptr;

    if (err) {
        request->send(400, "text/plain", "Bad Request JSON");
        return;
    }

    ParseResult<JsonRpcRequest> parsed = parseRequest(reqDoc);
    if (!parsed.ok) {
        JsonDocument errorDoc;
        writeError(errorDoc, reqDoc["id"], -32602, parsed.errorMessage);
        String output;
        serializeJson(errorDoc, output);
        request->send(400, "application/json", output);
        return;
    }

    const String accept = request->header("Accept");
    if (accept.indexOf("application/json") < 0 || accept.indexOf("text/event-stream") < 0) {
        request->send(406, "text/plain", "Accept must include application/json and text/event-stream");
        return;
    }

    const String protocolVersion = request->header("MCP-Protocol-Version");
    const String methodHeader = request->header("Mcp-Method");
    const String bodyVersion = parsed.data.params["_meta"]["io.modelcontextprotocol/protocolVersion"].as<String>();
    if (bodyVersion != McpServerInfo::supportedProtocols[0]) {
        JsonDocument errorDoc;
        writeError(errorDoc, reqDoc["id"], -32022, "Unsupported MCP protocol version");
        JsonArray supported = errorDoc["error"]["data"]["supported"].to<JsonArray>();
        for (const char* version : McpServerInfo::supportedProtocols) supported.add(version);
        errorDoc["error"]["data"]["requested"] = bodyVersion;
        String output;
        serializeJson(errorDoc, output);
        request->send(400, "application/json", output);
        return;
    }
    if (protocolVersion != bodyVersion || methodHeader != parsed.data.method) {
        JsonDocument errorDoc;
        writeError(errorDoc, reqDoc["id"], -32020, "MCP request headers do not match the request body");
        String output;
        serializeJson(errorDoc, output);
        request->send(400, "application/json", output);
        return;
    }

    const bool requiresName =
        strcmp(parsed.data.method, "tools/call") == 0 || strcmp(parsed.data.method, "resources/read") == 0;
    if (requiresName && !request->hasHeader("Mcp-Name")) {
        JsonDocument errorDoc;
        writeError(errorDoc, reqDoc["id"], -32020, "Mcp-Name header is required");
        String output;
        serializeJson(errorDoc, output);
        request->send(400, "application/json", output);
        return;
    }
    if (requiresName) {
        const String expectedName = strcmp(parsed.data.method, "tools/call") == 0
                                        ? parsed.data.params["name"].as<String>()
                                        : parsed.data.params["uri"].as<String>();
        if (request->header("Mcp-Name") != expectedName) {
            JsonDocument errorDoc;
            writeError(errorDoc, reqDoc["id"], -32020, "Mcp-Name header does not match the request body");
            String output;
            serializeJson(errorDoc, output);
            request->send(400, "application/json", output);
            return;
        }
    }

    if (parsed.data.id.isNull()) {
        handleMcpMessage(reqDoc, reqDoc);
        request->send(202);
        return;
    }

    if (McpRequestRegistry::handlers().find(parsed.data.method) == McpRequestRegistry::handlers().end()) {
        JsonDocument errorDoc;
        writeError(errorDoc, reqDoc["id"], -32601, "Method not found");
        String output;
        serializeJson(errorDoc, output);
        request->send(404, "application/json", output);
        return;
    }

    // Run MCP handler
    JsonDocument resDoc;
    handleMcpMessage(reqDoc, resDoc);

    String output;
    serializeJson(resDoc, output);
    request->send(200, "application/json", output);
};