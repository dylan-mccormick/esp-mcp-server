// main.cpp
// Main entrypoint to initialize all services

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>

#include "env.h"
#include "tasks/taskManager.h"
#include "web/router.h"

#ifndef WIFI_CONNECT_TIMEOUT_MS
#define WIFI_CONNECT_TIMEOUT_MS 30000
#endif

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    const unsigned long wifiStart = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < WIFI_CONNECT_TIMEOUT_MS) {
        delay(100);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("done!");
        Serial.print("Local IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("WiFi connection timed out.");
    }

    // Static file handling init
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS mount failed.");
        return;
    }

    // Start all background tasks
    startAllBackgroundTasks();

    // Start web server
    webServerInit();
}

void loop() {}