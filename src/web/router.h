// router.h
// Contains a header to run the web server task, which initializes routes for the web server

#include <ESPAsyncWebServer.h>

#ifndef WEB_SERVER_PORT
#define WEB_SERVER_PORT 80
#warning "Web server port is undefined. Defaulting to PORT 80."
#endif

extern ArRequestHandlerFunction staticHandler;
extern ArRequestHandlerFunction infoEndpointHandler;
extern ArRequestHandlerFunction mcpHandler;
extern ArRequestHandlerFunction notFoundHandler;

void webServerInit();