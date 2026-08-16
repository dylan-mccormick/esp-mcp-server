#include <ESPAsyncWebServer.h>

#include <vector>

#include "mcp/mcpDispatch.h"

struct MCPBodyBuffer {
    std::vector<uint8_t> data;
};

ArBodyHandlerFunction mcpHandler = [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index,
                                      size_t total) {
    // First chunk: alloc buffer to handle data
    if (index == 0) {
        auto* buffer = new MCPBodyBuffer();
        buffer->data.reserve(total);
        request->_tempObject = buffer;
    }

    // Insert data into buffer
    auto* buffer = static_cast<MCPBodyBuffer*>(request->_tempObject);
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

    // Run MCP handler
    JsonDocument resDoc;
    handleMcpMessage(reqDoc, resDoc);

    String output;
    serializeJson(resDoc, output);
    request->send(200, "application/json", output);
};