// tasks.h
// Header file containing method signatures for various FreeRTOS tasks and task manager methodss

void refreshFrontendTask(void* pvParameters);
void ntpSyncTask(void* pvParameters);

void startAllBackgroundTasks();