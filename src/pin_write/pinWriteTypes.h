// pinWriteTypes.h
// Contains useful structs and types for pinWrite operations

#pragma once

#include <cstdint>

namespace PinWrite {
struct PinOperation {
    uint8_t pin;
    int delayAfter;

    bool digital;
    uint16_t value; // if digital is true, 0 for LOW, 1 for HIGH
};

struct ValidationResult {
    bool ok;
    const char* error;

    PinOperation operation;

    static ValidationResult errorResult(const char* method);
};
} // namespace PinWrite
