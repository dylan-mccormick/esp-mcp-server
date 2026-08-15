// componentMemory.cpp
// Implementation for the ComponentMemory namespace

#include "mcp/registries/mcpResourceRegistry.h"
#include "componentMemory.h"

static Preferences prefs;
static const char* NVS_NAMESPACE = "components";
static const char* NVS_KEY = "data";

void ComponentMemory::remember(int pin, const String& name, const String& description) {
    // Access memory
    prefs.begin(NVS_NAMESPACE);
    String raw = prefs.getString(NVS_KEY, "[]");

    // Deserialize current Json
    JsonDocument doc;
    deserializeJson(doc, raw);
    JsonArray arr = doc.as<JsonArray>();

    // Replace entry for pin, else append
    bool found = false;
    for (JsonObject entry : arr) {
        if (entry["pin"] != pin) continue;

        entry["name"] = name;
        entry["notes"] = description;
        found = true;
        break;
    }

    if (!found) {
        JsonObject newPin = arr.add<JsonObject>();
        newPin["pin"] = pin;
        newPin["name"] = name;
        newPin["notes"] = description;
    }

    // Serialize and update NVS
    String updated;
    serializeJson(doc, updated);
    prefs.putString(NVS_KEY, updated);
    prefs.end();
}

void ComponentMemory::forget(int pin) {
    // Access memory
    prefs.begin(NVS_NAMESPACE);
    String raw = prefs.getString(NVS_KEY, "[]");

    // Deserialize current Json
    JsonDocument doc;
    deserializeJson(doc, raw);
    JsonArrayConst arr = doc.as<JsonArrayConst>();

    // Copy the arr without the specified pin
    JsonDocument filtered;
    JsonArray filteredArr = filtered.to<JsonArray>();
    for (JsonObjectConst entry : arr) {
        if (entry["pin"] == pin) continue;

        JsonObject copyObj = filteredArr.add<JsonObject>();
        copyObj["pin"] = entry["pin"];
        copyObj["name"] = entry["name"];
        copyObj["notes"] = entry["notes"];
    }

    // Update
    String updated;
    serializeJson(filtered, updated);
    prefs.putString(NVS_KEY, updated);
    prefs.end();
}

void ComponentMemory::listAll(JsonArray out) {
    // Access memory
    prefs.begin(NVS_NAMESPACE, true);
    String raw = prefs.getString(NVS_KEY, "[]");
    prefs.end();

    JsonDocument doc;
    deserializeJson(doc, raw);
    for (JsonObjectConst entry : doc.as<JsonArrayConst>()) {
        JsonObject copy = out.add<JsonObject>();
        copy["pin"] = entry["pin"];
        copy["name"] = entry["name"];
        copy["notes"] = entry["notes"];
    }
}

// MCP Resource Handler
void handleReadComponentMemory(JsonVariant result) {
    JsonObject obj = result["contents"].add<JsonObject>();

    obj["uri"] = "esp32://components";
    obj["mimeType"] = "application/json";

    // List All
    JsonDocument textDoc;
    JsonArray arr = textDoc.to<JsonArray>();
    ComponentMemory::listAll(arr);
    String text;
    serializeJson(textDoc, text);
    obj["text"] = text;

    // Cache
    result["ttlMs"] = 120000;
    result["cacheScope"] = "public";
}

MCP_RESOURCE_DEF(
    "esp32://components",
    "Connected Pin Descriptions",
    "Provides a list of name/description pairs for the ESP32's pins, describing what is connected to which pins.",
    "application/json",
    handleReadComponentMemory
);