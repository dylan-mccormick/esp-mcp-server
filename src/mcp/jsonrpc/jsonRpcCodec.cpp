// jsonRpcCodec.cpp
// Implementation for methods described in jsonRpcCodec.h

#include <ArduinoJson.h>

#include "config/jsonRpcConfig.h"
#include "env.h"
#include "jsonRpcTypes.h"

ParseResult<JsonRpcRequest> parseRequest(const JsonDocument& doc) {
    JsonRpcRequest req;

    if (doc["jsonrpc"] != JSON_RPC_PROTOCOL_VERSION) {
        return {false, {}, "jsonrpc must be 2.0"};
    }
    if (!doc["method"].is<const char*>()) {
        return {false, {}, "method must be a string"};
    }

    const bool hasId = doc.containsKey("id");
    if (hasId && (!doc["id"].is<long>() && !doc["id"].is<const char*>() && !doc["id"].is<int64_t>())) {
        return {false, {}, "id must be a string or integer"};
    }

    if (!doc["params"].isNull() && !doc["params"].is<JsonObjectConst>()) {
        return {false, {}, "params must be an object"};
    }

    JsonObjectConst params = doc["params"].as<JsonObjectConst>();
    JsonObjectConst meta = params["_meta"].as<JsonObjectConst>();
    if (!meta["io.modelcontextprotocol/protocolVersion"].is<const char*>()) {
        return {false, {}, "missing protocol version metadata"};
    }
    if (!meta["io.modelcontextprotocol/clientCapabilities"].is<JsonObjectConst>()) {
        return {false, {}, "missing client capabilities metadata"};
    }

    req.id = doc["id"];
    req.method = doc["method"].as<const char*>();
    req.params = params;

    return {true, req, nullptr};
}

void writeResult(JsonDocument& doc, const JsonRpcResult& result) {
    doc["id"] = result.id;
    doc["jsonrpc"] = result.jsonrpc;
    doc["result"]["resultType"] = jsonRpcResultTypeToString(result.resultType);
}

void writeError(JsonDocument& doc, const JsonVariantConst id, const int errorCode, const char* errorMessage) {
    JsonObject errObj = doc["error"].to<JsonObject>();

    errObj["code"] = errorCode;
    errObj["message"] = errorMessage;

    doc["id"] = id;
    doc["jsonrpc"] = JSON_RPC_PROTOCOL_VERSION;
}