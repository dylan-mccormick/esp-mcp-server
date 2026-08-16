// componentMemory.h
// Headers for the component memory--allows the MCP server to describe which components are
// connected to which pins.

#pragma once
#include <ArduinoJson.h>
#include <Preferences.h>

namespace ComponentMemory {
void remember(int pin, const String& name, const String& notes);
void forget(int pin);
void listAll(JsonArray out);
}  // namespace ComponentMemory