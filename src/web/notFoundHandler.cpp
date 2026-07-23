#include <ESPAsyncWebServer.h>

ArRequestHandlerFunction notFoundHandler = [](AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "Page Not Found");
};