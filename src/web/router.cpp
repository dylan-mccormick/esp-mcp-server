// router.cpp
// Provides an implementation to initialize the web server

#include <ESPAsyncWebServer.h>
#include "router.h"

AsyncWebServer server(80);

void webServerInit() {
    server.on("/", HTTP_GET, staticHandler);
    server.onNotFound(notFoundHandler);
    server.begin();
}