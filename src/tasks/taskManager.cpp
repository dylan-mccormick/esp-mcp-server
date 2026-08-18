// taskManager.cpp
// Contains logic to manage all FreeRTOS task definitions

#include <Arduino.h>

#include "refreshFrontendTask.h"
#include "pinWriteSequenceScheduler.h"

void startAllBackgroundTasks() {
    startRefreshFrontendTask();
    startPinWriteSequenceSchedulerTask();
}