#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

#include "env.h"

const char* CHIP_MODEL = ESP.getChipModel();

JsonDocument infoEndpointResponse;

ArRequestHandlerFunction infoEndpointHandler = [](AsyncWebServerRequest* request) {
    infoEndpointResponse["deviceName"] = CHIP_MODEL;
    infoEndpointResponse["wifiSSID"] = WIFI_SSID;
    infoEndpointResponse["mcpEndpoint"] = "/mcp";

    String result;
    serializeJson(infoEndpointResponse, result);

    request->send(200, "application/json", result);
};