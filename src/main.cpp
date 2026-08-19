// main.cpp
// Main entrypoint to initialize all services

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>

#include "env.h"
#include "tasks/taskManager.h"
#include "web/router.h"

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println("done!");
    Serial.print("Local IP address: ");
    Serial.println(WiFi.localIP());

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