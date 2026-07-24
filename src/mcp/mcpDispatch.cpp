// mcpDispatch.cpp
// Contains method(s) to implement signatures in mcpDispatch.h. Contains additonal helper methods

#include <ArduinoJson.h>
#include "mcp/jsonrpc/jsonRpcTypes.h"
#include "mcp/jsonrpc/jsonRpcCodec.h"

void handleMcpNotification(const JsonRpcRequest& req) {

}

void handleMcpRequest(const JsonRpcRequest& req, JsonRpcResult& res) {

}

void handleMcpMessage(const JsonDocument& reqDoc, JsonDocument& resDoc) {
    // parse as a JSON-RPC request
    ParseResult<JsonRpcRequest> request = parseRequest(reqDoc);
    if (!request.ok) {
        writeError(resDoc, reqDoc["id"], -32600, request.errorMessage);
        return;
    }

    const JsonRpcRequest& req = request.data;

    if (req.id.isNull()) { // this is an MCP notification
        handleMcpNotification(req);
        return;
    }

    JsonRpcResult res;
    handleMcpRequest(req, res);
    writeResult(resDoc, res);
}