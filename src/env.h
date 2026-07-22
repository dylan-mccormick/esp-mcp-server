// env.h
// Contains preprocessor macros to ensure all environment variables are defined at compile-tile

#pragma once

// verify wifi credentials are defined
#ifndef WIFI_SSID
#error "WIFI_SSID is not defined. Check that .env exists and contains WIFI_SSID"
#endif
#ifndef WIFI_PWD
#error "WIFI_PWD is not defined. Check that .env exists and contains WIFI_PWD"
#endif