// pinWriteScheduler.h
// Contains variables and methods useful for modules accessing the pin write scheduler

#pragma once

#include <freertos/task.h>
#include <freertos/queue.h>

extern TaskHandle_t pinWriteSequenceTask;
extern QueueHandle_t pinWriteSequenceQueue;

void startPinWriteSequenceSchedulerTask();