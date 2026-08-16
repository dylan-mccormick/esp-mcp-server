// taskManager.cpp
// Contains logic to manage all FreeRTOS task definitions

#include <Arduino.h>

#include "tasks.h"

void startAllBackgroundTasks() {
    xTaskCreate(refreshFrontendTask, "RefreshFrontendTask", 16384, nullptr, 1, nullptr);
}