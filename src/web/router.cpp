// router.cpp
// Provides an implementation to initialize the web server

#include <ESPAsyncWebServer.h>
#include "router.h"

#ifndef RATE_LIMIT_MAX_REQUESTS
#define RATE_LIMIT_MAX_REQUESTS 25
#endif
#ifndef RATE_LIMIT_WINDOW
#define RATE_LIMIT_WINDOW 10
#endif

#ifdef DEV_MODE
#warning "DEV_MODE build flag is enabled, meaning the server is vulnerable to DNS rebinding attacks. Remove this build flag when not testing."
#else
#define DEV_MODE false
#endif

AsyncWebServer server(80);
AsyncRateLimitMiddleware rateLimit;
AsyncLoggingMiddleware logging;
AsyncCorsMiddleware cors;

void webServerInit() {
    // HTTP request logger
    logging.setEnabled(true);
    logging.setOutput(Serial);
    server.addMiddleware(&logging);

    // TRate Limiting
    rateLimit.setMaxRequests(RATE_LIMIT_MAX_REQUESTS);
    rateLimit.setWindowSize(RATE_LIMIT_WINDOW);
    server.addMiddleware(&rateLimit);

    // CORS global allow
    cors.setOrigin("*");
    cors.setMethods("GET,POST,OPTIONS");
    cors.setHeaders("Content-Type,Authorization,Mcp-Method,Mcp-Protocol-Version,Mcp-Name");
    server.addMiddleware(&cors);

    // Origin validation logger
    server.addMiddleware([](AsyncWebServerRequest* request, ArMiddlewareNext next) {
        if (!request->hasHeader("Origin") || DEV_MODE) {
            // if DEV_MODE, we may be testing the MCP client from a different IP address -- only permissible for development
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
    server.on("/info", HTTP_GET, infoEndpointHandler);
    server.on("/connections", HTTP_GET, staticHandler); // in case they open the connections manager
    server.on("/mcp", HTTP_POST, ([](AsyncWebServerRequest* req) {
        // Do Nothing'
    }), nullptr, mcpHandler);
    server.begin();

}