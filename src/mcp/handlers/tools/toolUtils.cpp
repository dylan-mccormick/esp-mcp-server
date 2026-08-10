// toolUtils.cpp
// Contains implementations for toolUtils.h

#include <ArduinoJson.h>

void writeToolError(JsonVariant res, const char* message) {
    res["isError"] = true;
    JsonArray contentArr = res["content"].to<JsonArray>();
    JsonObject contentBlock = contentArr.add<JsonObject>();
    contentBlock["type"] = "text";
    contentBlock["text"] = message;
}

void writeToolSuccess(JsonVariant res, const char* message) {
    res["isError"] = false;
    JsonArray contentArr = res["content"].to<JsonArray>();
    JsonObject contentBlock = contentArr.add<JsonObject>();
    contentBlock["type"] = "text";
    contentBlock["text"] = message;
}