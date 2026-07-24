// jsonRpcCodec.h
// Specifies utility methods for parsing and serializing JSON into specific JSON-RPC structs

#include <ArduinoJson.h>
#include "jsonRpcTypes.h"

ParseResult<JsonRpcRequest> parseRequest(const JsonDocument& doc);
void writeResult(JsonDocument& doc, const JsonRpcResult& result);