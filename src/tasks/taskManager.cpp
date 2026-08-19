// taskManager.cpp
// Contains logic to manage all FreeRTOS task definitions

#include "refreshFrontendTask.h"
#include "pinWriteSequenceScheduler.h"

void startAllBackgroundTasks() {
    startRefreshFrontendTask();
    startPinWriteSequenceSchedulerTask();
}