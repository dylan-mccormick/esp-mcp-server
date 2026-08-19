// pinWriteUtils.h
// Contains several shared utility functions for handling pin write MCP tools

#pragma once

#include <ArduinoJson.h>
#include <unordered_set>

#include "pinWriteTypes.h"

namespace PinWrite {
// Constants
const static std::unordered_set<uint8_t> PROHIBITED_PINS = {0,  1,  3,  6,  7,  8,  9,  10, 11, 20, 24,
                                                            28, 29, 30, 31, 34, 35, 36, 37, 38, 39};

// Methods
ValidationResult validatePinArguments(const JsonObjectConst args);
void handlePinOperations(const PinOperation& info);
};  // namespace PinWrite