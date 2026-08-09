// toolUtils.cpp
// Contains implementations for toolUtils.h

#include <ArduinoJson.h>

void writeToolError(JsonVariant res, const char* message) {
    res["isError"] = true;
    JsonArray contentArr = res["content"].as<JsonArray>();
    JsonObject contentBlock = contentArr.add<JsonObject>();
    contentBlock["type"] = "text";
    contentBlock["text"] = message;
}