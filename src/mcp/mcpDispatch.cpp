// mcpDispatch.cpp
// Contains method(s) to implement signatures in mcpDispatch.h. Contains additonal helper methods

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include "registries/mcpRequestRegistry.h"
#include "registries/mcpNotificationRegistry.h"
#include "mcp/jsonrpc/jsonRpcTypes.h"
#include "mcp/jsonrpc/jsonRpcCodec.h"

void handleMcpMessage(const JsonDocument& reqDoc, JsonDocument& resDoc) {
    // parse as a JSON-RPC request
    ParseResult<JsonRpcRequest> request = parseRequest(reqDoc);
    if (!request.ok) {
        writeError(resDoc, reqDoc["id"], -32600, request.errorMessage);
        return;
    }

    const JsonRpcRequest& req = request.data;

    if (req.id.isNull()) { // this is an MCP notification
        auto& handlers = McpNotificationRegistry::handlers();
        auto target = handlers.find(req.method);
        if (target == handlers.end()) return; // not expected to send a response anyway
        target->second(req.params);
        return;
    }

    auto& handlers = McpRequestRegistry::handlers();
    auto target = handlers.find(req.method);
    if (target == handlers.end()) {
        writeError(resDoc, req.id, -32601, "Method not found");
        return;
    }

    JsonRpcResult res;
    res.result = resDoc["result"].to<JsonVariant>();
    res.id = req.id; // echo id, we don't have to manually process this every time
    target->second(req.params, res.result);
    writeResult(resDoc, res);
}