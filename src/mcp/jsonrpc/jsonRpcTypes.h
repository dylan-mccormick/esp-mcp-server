// jsonRpcTypes.h
// Defines structs for the JSON-RPC protocol

#pragma once

#include <ArduinoJson.h>
#include "config/jsonRpcConfig.h"

template<typename T>
struct ParseResult {
    bool ok;
    T data;
    const char* errorMessage;
};

struct BaseJsonRpcMessage {
    const char* jsonrpc = JSON_RPC_PROTOCOL_VERSION;
    JsonVariantConst id;
};

struct JsonRpcRequest : BaseJsonRpcMessage {
    const char* method;
    JsonObjectConst params;
};

enum JsonRpcResultType {
    COMPLETE,
    INPUT_REQUIRED
};

inline const char* jsonRpcResultTypeToString(JsonRpcResultType resultType) {
    switch (resultType) {
        case JsonRpcResultType::COMPLETE: return "complete";
        case JsonRpcResultType::INPUT_REQUIRED: return "input_required";
    }
    return "";
}

struct JsonRpcResult : BaseJsonRpcMessage {
    JsonRpcResultType resultType = JsonRpcResultType::COMPLETE;
    JsonVariant result;
};