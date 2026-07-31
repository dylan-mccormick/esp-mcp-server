// jsonRpcCodec.cpp
// Implementation for methods described in jsonRpcCodec.h

#include <ArduinoJson.h>
#include "jsonRpcTypes.h"
#include "env.h"

#ifndef JSON_RPC_PROTOCOL_VERSION
#warning "JSON_RPC_PROTOCOL_VERSION is not specified in environment file. Defaulting to 2.0."
#define JSON_RPC_PROTOCOL_VERSION "2.0"
#endif

ParseResult<JsonRpcRequest> parseRequest(const JsonDocument& doc) {
    JsonRpcRequest req;

    // validation
    if (!doc["id"].is<long>() && !doc["id"].is<const char*>()) {
        return { false, {}, "id must be a string or number" };
    }
    if (!doc["method"].is<const char*>()) {
        return { false, {}, "method must be a string" };
    }

    req.id = doc["id"];
    req.method = doc["method"].as<const char*>();
    req.params = doc["params"].as<JsonObjectConst>();

    return { true, req, nullptr };
}

void writeResult(JsonDocument& doc, const JsonRpcResult& result) {
    doc["id"] = result.id;
    doc["jsonrpc"] = result.jsonrpc;
    doc["result"] = result.result;
}

void writeError(JsonDocument& doc, const JsonVariantConst id, const int errorCode, const char* errorMessage) {
    JsonObject errObj = doc["error"].to<JsonObject>();

    errObj["code"] = errorCode;
    errObj["message"] = errorMessage;

    doc["id"] = id;
    doc["jsonrpc"] = JSON_RPC_PROTOCOL_VERSION;
}