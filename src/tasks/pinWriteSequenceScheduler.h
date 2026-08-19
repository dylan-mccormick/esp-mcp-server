// pinWriteScheduler.h
// Contains variables and methods useful for modules accessing the pin write scheduler

#pragma once

#include <queue>
#include <variant>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "pin_write/pinWriteTypes.h"

using PinWriteSequence = std::queue<PinWrite::PinOperation>;

bool submitPinWriteSequence(PinWriteSequence&& sequence);
void startPinWriteSequenceSchedulerTask();