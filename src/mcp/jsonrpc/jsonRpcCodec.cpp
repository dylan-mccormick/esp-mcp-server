// jsonRpcCodec.cpp
// Implementation for methods described in jsonRpcCodec.h

#include <ArduinoJson.h>

#include "config/jsonRpcConfig.h"
#include "env.h"
#include "jsonRpcTypes.h"

ParseResult<JsonRpcRequest> parseRequest(const JsonDocument& doc) {
    JsonRpcRequest req;

    // validation
    if (!doc["id"].is<long>() && !doc["id"].is<const char*>()) {
        return {false, {}, "id must be a string or number"};
    }
    if (!doc["method"].is<const char*>()) {
        return {false, {}, "method must be a string"};
    }

    req.id = doc["id"];
    req.method = doc["method"].as<const char*>();
    req.params = doc["params"].as<JsonObjectConst>();

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