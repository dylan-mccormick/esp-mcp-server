#pragma once

// verify wifi credentials are defined
#ifndef WIFI_SSID
// NOLINTNEXTLINE
#error "WIFI_SSID is not defined. Check that .env exists and contains WIFI_SSID"
#endif
#ifndef WIFI_PWD
// NOLINTNEXTLINE
#error "WIFI_PWD is not defined. Check that .env exists and contains WIFI_PWD"
#endif