#ifndef SmarthomeService_h
#define SmarthomeService_h
#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"

class SmartHomeService
{
    public:
        SmartHomeService();
        void UpdateIpAddress(String ipAddress);

    private:
        HTTPClient _client;
        WiFiClient _wifiClient;
};

#endif