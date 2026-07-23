// router.cpp
// Provides an implementation to initialize the web server

#include <ESPAsyncWebServer.h>
#include "router.h"

#ifndef RATE_LIMIT_MAX_REQUESTS
#define RATE_LIMIT_MAX_REQUESTS 5
#endif
#ifndef RATE_LIMIT_WINDOW
#define RATE_LIMIT_WINDOW 10
#endif

AsyncWebServer server(80);
AsyncRateLimitMiddleware rateLimit;
AsyncLoggingMiddleware logging;

void webServerInit() {

    // HTTP request logger
    logging.setEnabled(true);
    logging.setOutput(Serial);
    server.addMiddleware(&logging);

    // TRate Limiting
    rateLimit.setMaxRequests(RATE_LIMIT_MAX_REQUESTS);
    rateLimit.setWindowSize(RATE_LIMIT_WINDOW);
    server.addMiddleware(&rateLimit);

    // Origin validation logger
    server.addMiddleware([](AsyncWebServerRequest* request, ArMiddlewareNext next) {
        if (!request->hasHeader("Origin")) {
            // if origin is not present, we will allow this, since we are concerned primarily about
            // DNS rebinding attacks
            next();
            return;
        }

        String origin = request->header("Origin");
        String host = request->host();

        // strip origin
        int schemeEnd = origin.indexOf("://");
        String originHostname = (schemeEnd >= 0) ? origin.substring(schemeEnd + 3) : origin;

        // validate
        if (originHostname != host) {
            request->send(403, "text/plain", "Forbidden");
            return;
        }

        next();
    });

    server.on("/", HTTP_GET, staticHandler);
    server.onNotFound(notFoundHandler);
    server.begin();

}