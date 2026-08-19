// taskManager.cpp
// Contains logic to manage all FreeRTOS task definitions

#include "pinWriteSequenceScheduler.h"
#include "refreshFrontendTask.h"

void startAllBackgroundTasks() {
    startRefreshFrontendTask();
    startPinWriteSequenceSchedulerTask();
}