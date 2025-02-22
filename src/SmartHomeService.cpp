#include "SmarthomeService.h"


SmartHomeService::SmartHomeService() : _client(), _wifiClient()
{
    
}

bool SmartHomeService::UpdateIpAddress(String ipAddress)
{
    Serial.println("Updating smarthome");

    String url = "http://homeassistant.local:8123/api/events/general_update_microcontroller_ip_address";
    
    // Initialize HTTPClient with the WiFiClient instance
    _client.begin(_wifiClient, url);  // Begin HTTPClient with WiFiClient and the URL

    // Authorization header
    String authHeader = "Bearer myToken";
    _client.addHeader("Authorization", authHeader);
    _client.addHeader("Content-Type", "application/json");

    // JSON body with the IP address
    String jsonBody = "{\"staticName\": \"garage_central_sensor_microcontroller\", \"baseIpAddress\": \"" + ipAddress + "\", \"healthCheckRoute\": \"/health\"}";

    // Make the POST request
    int httpResponseCode = _client.POST(jsonBody);

    // End the HTTP request
    _client.end();

    return (httpResponseCode == 200);
}