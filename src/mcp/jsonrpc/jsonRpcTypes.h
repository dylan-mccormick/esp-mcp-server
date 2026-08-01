// jsonRpcTypes.h
// Defines structs for the JSON-RPC protocol

#pragma once

#include <ArduinoJson.h>

template<typename T>
struct ParseResult {
    bool ok;
    T data;
    const char* errorMessage;
};

struct BaseJsonRpcMessage {
    const char* jsonrpc = "2.0";
    JsonVariantConst id;
};

struct JsonRpcRequest : BaseJsonRpcMessage {
    const char* method;
    JsonObjectConst params;
};

struct JsonRpcResult : BaseJsonRpcMessage {
    JsonVariant result;
};