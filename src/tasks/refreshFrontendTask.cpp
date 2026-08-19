// refreshFrontendTask.cpp
// For loading a fresh copy of the MCP server frontend chat system into the filesystem

#include <Arduino.h>
#include <HTTPClient.h>
#include <LittleFS.h>

#include "refreshFrontendTask.h"

#define FRONTEND_PAGE_SOURCE_URL "https://registry-api.mnmzc.us.to/r/7/api/v1/public/frontend/index.html"
#define FRONTEND_PAGE_CACHE_TTL 5 * 60 * 1000  // 5 minutes

void refreshFrontendTask(void* pvParameters) {
    for (;;) {
        // Open the file
        File file = LittleFS.open("/app.html", "w");
        if (!file) {
            Serial.println("Failed to open /app.html for writing.");
            vTaskDelay(pdMS_TO_TICKS(FRONTEND_PAGE_CACHE_TTL));
            continue;
        }

        // make the request
        HTTPClient http;
        http.begin(FRONTEND_PAGE_SOURCE_URL);
        int httpCode = http.GET();

        // parse results
        if (httpCode == HTTP_CODE_OK) {
            http.writeToStream(&file);
            Serial.println("Successfully saved new frontend page.");
        } else {
            Serial.printf("Frontend page fetch GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
        file.close();
        vTaskDelay(pdMS_TO_TICKS(FRONTEND_PAGE_CACHE_TTL));
    }
}

void startRefreshFrontendTask() {
    xTaskCreate(refreshFrontendTask, "RefreshFrontendTask", 16384, nullptr, 1, nullptr);
}