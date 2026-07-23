#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>

ArRequestHandlerFunction staticHandler = [](AsyncWebServerRequest* request) {
    // Open the app.html file (for controlling the MCP Server)
    File file = LittleFS.open("/app.html");
    if (!file) {
        Serial.println("[Error] Could not serve static web app: file failed to open.");
        request->send(500, "text/plain", "Internal Server Error");
        return;
    }

    AsyncWebServerResponse* response = request->beginChunkedResponse("text/html", [file](uint8_t* buffer, size_t maxLen, size_t index) mutable -> size_t {
        if (!file.available()) {
            file.close();
            return 0;
        }

        return file.read(buffer, maxLen);
    });

    request->send(response);
};