// refreshFrontendTask.cpp
// For loading a fresh copy of the MCP server frontend chat system into the filesystem

#include "refreshFrontendTask.h"

#include <Arduino.h>
#include <HTTPClient.h>
#include <LittleFS.h>

#ifndef FRONTEND_PAGE_SOURCE_URL
#define FRONTEND_PAGE_SOURCE_URL "https://registry-api.mnmzc.us.to/r/7/api/v1/public/frontend/index.html"
#endif
#ifndef FRONTEND_PAGE_CACHE_TTL_MS
#define FRONTEND_PAGE_CACHE_TTL_MS (5UL * 60UL * 1000UL)
#endif
#ifndef FRONTEND_PAGE_MAX_SIZE
#define FRONTEND_PAGE_MAX_SIZE 262144
#endif
#ifndef FRONTEND_HTTP_TIMEOUT_MS
#define FRONTEND_HTTP_TIMEOUT_MS 10000
#endif

void refreshFrontendTask(void* pvParameters) {
    for (;;) {
        File file = LittleFS.open("/app.html.tmp", "w");
        if (!file) {
            vTaskDelay(pdMS_TO_TICKS(FRONTEND_PAGE_CACHE_TTL_MS));
            continue;
        }

        // make the request
        HTTPClient http;
        http.setTimeout(FRONTEND_HTTP_TIMEOUT_MS);
        http.begin(FRONTEND_PAGE_SOURCE_URL);
        int httpCode = http.GET();

        // parse results
        const int contentLength = http.getSize();
        if (httpCode == HTTP_CODE_OK && contentLength >= 0 && contentLength <= FRONTEND_PAGE_MAX_SIZE) {
            http.writeToStream(&file);
            file.close();
            File downloadedFile = LittleFS.open("/app.html.tmp", "r");
            const size_t downloadedSize = downloadedFile ? downloadedFile.size() : 0;
            downloadedFile.close();
            if (downloadedFile && downloadedSize <= FRONTEND_PAGE_MAX_SIZE) {
                LittleFS.remove("/app.html");
                LittleFS.rename("/app.html.tmp", "/app.html");
            } else {
                LittleFS.remove("/app.html.tmp");
            }
        } else {
            file.close();
            LittleFS.remove("/app.html.tmp");
        }

        http.end();
        vTaskDelay(pdMS_TO_TICKS(FRONTEND_PAGE_CACHE_TTL_MS));
    }
}

void startRefreshFrontendTask() {
    xTaskCreate(refreshFrontendTask, "RefreshFrontendTask", 16384, nullptr, 1, nullptr);
}