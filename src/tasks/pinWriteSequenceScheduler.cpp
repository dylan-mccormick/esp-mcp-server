// pinWriteSequenceScheduler.cpp
// Scheduler to handle pinWriteSequence tool calls. Tool may send arrays of operations to this task's queue, which will
// be scheduled and executed.

#include "pinWriteSequenceScheduler.h"

#include "pin_write/pinWriteTypes.h"
#include "pin_write/pinWriteUtils.h"

TaskHandle_t pinWriteSequenceTask = nullptr;
QueueHandle_t bufferQueue = nullptr;

struct ScheduledPinOperation {
    uint8_t pin;
    bool digital;
    uint16_t value;
    TickType_t runAfter;
};

auto cmp = [](ScheduledPinOperation* a, ScheduledPinOperation* b) { return a->runAfter > b->runAfter; };
using pinPriorityQueue =
    std::priority_queue<ScheduledPinOperation*, std::vector<ScheduledPinOperation*>, decltype(cmp)>;

// Extract pin operations and add to queue
void processSequence(PinWriteSequence* sequence, pinPriorityQueue& queue) {
    TickType_t nextTickCount = xTaskGetTickCount();

    while (!sequence->empty()) {
        PinWrite::PinOperation target = sequence->front();

        ScheduledPinOperation* ptr =
            new ScheduledPinOperation({target.pin, target.digital, target.value, nextTickCount});
        const TickType_t delayTicks = pdMS_TO_TICKS(target.delayAfter);
        if (nextTickCount > portMAX_DELAY - delayTicks) {
            nextTickCount = portMAX_DELAY;
        } else {
            nextTickCount += delayTicks;
        }

        queue.push(ptr);
        sequence->pop();
    }

    delete sequence;
}

void pinWriteSchedulerTask(void* pvParameters) {
    pinPriorityQueue operations(cmp);

    for (;;) {
        // Execute the next operation
        if (!operations.empty()) {
            ScheduledPinOperation* top = operations.top();
            if (top->runAfter <= xTaskGetTickCount()) {
                PinWrite::handlePinOperations({top->pin, 0, top->digital, top->value});
                delete top;
                operations.pop();
            }
        }

        // Receive new data from bufferQueue
        PinWriteSequence* recv;
        if (xQueueReceive(bufferQueue, &recv, 0) == pdTRUE) {
            processSequence(recv, operations);
        }

        // Wait til notify or next scheduled operation
        TickType_t now = xTaskGetTickCount();
        ScheduledPinOperation* nextTop = operations.empty() ? nullptr : operations.top();
        ulTaskNotifyTake(pdTRUE, nextTop == nullptr
                                     ? portMAX_DELAY
                                     : (nextTop->runAfter > now ? nextTop->runAfter - now
                                                                : 0));  // wait 0 ticks if the time has already passed
    }
}

bool submitPinWriteSequence(PinWriteSequence&& sequence) {
    // Own the sequence so we can delete when done
    PinWriteSequence* ptr = new PinWriteSequence(std::move(sequence));

    if (xQueueSend(bufferQueue, &ptr, 0) != pdTRUE) {
        delete ptr;
        return false;
    }

    xTaskNotifyGive(pinWriteSequenceTask);
    return true;
}

void startPinWriteSequenceSchedulerTask() {
    bufferQueue = xQueueCreate(10, sizeof(PinWriteSequence*));

    xTaskCreate(pinWriteSchedulerTask, "PinWriteSequenceScheduler", 4096, nullptr, 1, &pinWriteSequenceTask);
}