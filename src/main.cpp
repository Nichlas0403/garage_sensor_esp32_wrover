#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "GpioService.h"

//GPIOs
#define _dht11Input 32 //DHT11 Input
#define _photoresistorInput 33 //Read from photoressitor
#define _photoresistorOutput 17 //Output voltage to photoresistor so value can be read

// Cores services
AsyncWebServer _server(80);
GpioService _gpioService(_photoresistorInput, _photoresistorOutput, _dht11Input);


//Endpoints
void GetHealth(AsyncWebServerRequest *request);
void ToggleOn(AsyncWebServerRequest *request);
void ToggleOff(AsyncWebServerRequest *request);

//Server setup
void RestServerRouting();
void HandleNotFound(AsyncWebServerRequest *request);
void ConnectToWiFi(String ssid, String password);

void setup() 
{
  Serial.begin(9600);
  String ssid = "SSID";
  String wifiPassword = "Password";

  ConnectToWiFi(ssid, wifiPassword);

}

void loop() 
{
}

void GetHealth(AsyncWebServerRequest *request) 
{
  request->send(200);
}

void GetPhotoresistorReading(AsyncWebServerRequest *request)
{
  int value = _gpioService.GetPhotoresistorValue();

  // Create a JSON document
  DynamicJsonDocument doc(1024);
  doc["value"] = value;
  doc["dataType"] = "lux";

  // Serialize JSON document to a string
  String responseBody;
  serializeJson(doc, responseBody);

  // Send the response with application/json content type
  request->send(200, "application/json", responseBody);
}

void GetTemperatureReading(AsyncWebServerRequest *request)
{
    float value = _gpioService.GetTemperature();

  // Create a JSON document
  DynamicJsonDocument doc(1024);
  doc["value"] = value;
  doc["dataType"] = "celcius";

  // Serialize JSON document to a string
  String responseBody;
  serializeJson(doc, responseBody);

  // Send the response with application/json content type
  request->send(200, "application/json", responseBody);
}

void GetHumidityReading(AsyncWebServerRequest *request)
{
    float value = _gpioService.GetHumidity();

  // Create a JSON document
  DynamicJsonDocument doc(1024);
  doc["value"] = value;
  doc["dataType"] = "Percentage";

  // Serialize JSON document to a string
  String responseBody;
  serializeJson(doc, responseBody);

  // Send the response with application/json content type
  request->send(200, "application/json", responseBody);
}

// Endpoints
void RestServerRouting() 
{
  _server.on("/health", HTTP_GET, GetHealth);
  _server.on("/photoresistor", HTTP_GET, GetPhotoresistorReading);
  _server.on("/temperature", HTTP_GET, GetTemperatureReading);
  _server.on("/humidity", HTTP_GET, GetHumidityReading);
}

void HandleNotFound(AsyncWebServerRequest *request) 
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";

  for (uint8_t i = 0; i < request->args(); i++) 
  {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }

  request->send(404, "text/plain", message);
}

void ConnectToWiFi(String ssid, String password) 
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Activate mDNS with a hostname
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  // Set server routing
  RestServerRouting();
  // Set not found response
  _server.onNotFound(HandleNotFound);
  // Start server
  _server.begin();

  Serial.println("HTTP server started");
}