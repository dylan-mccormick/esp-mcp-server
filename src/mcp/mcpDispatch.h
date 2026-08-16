// mcpDispatch.h
// Contains headers for the MCP Server dispatch system, which is responsible for taking JSON-RPC input values (from web
// requests) and routing them to the correct method

#pragma once

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

#include "jsonrpc/jsonRpcTypes.h"

void handleMcpMessage(const JsonDocument& reqDoc, JsonDocument& resDoc);