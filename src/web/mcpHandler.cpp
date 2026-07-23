#include <ESPAsyncWebServer.h>

ArRequestHandlerFunction mcpHandler = [](AsyncWebServerRequest* request) {
    request->send(501, "text/plain", "not implemented");
};