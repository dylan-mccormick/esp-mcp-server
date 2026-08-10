// toolUtils.h
// Contains definitions for utility functions for MCP tools

#pragma once

#include <ArduinoJson.h>

template<typename T>
bool requireArg(JsonObjectConst args, const char* name, T& out) {
    if (!args[name].is<T>()) return false;
    out = args[name].as<T>();
    return true;
}

void writeToolError(JsonVariant res, const char* message);
void writeToolSuccess(JsonVariant res, const char* message);