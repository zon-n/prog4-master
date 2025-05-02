#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <Arduino_JSON.h>

// Replace with your network credentials
const char *ssid = "DESKTOP-MC5O0GI 6923";
const char *password = "22]j8M81";

AsyncWebServer server(80);
AsyncEventSource events("/events");
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

// Get Sensor Readings and return JSON object
String getSensorReadings()
{
    readings["sensor1"] = random(0, 10);
    readings["sensor2"] = random(0, 10);
    readings["sensor3"] = random(0, 10);
    readings["sensor4"] = random(0, 10);

    String jsonString = JSON.stringify(readings);
    return jsonString;
}

// Initialize LittleFS File System
void initLittleFS()
{
    if (!LittleFS.begin())
    {
        Serial.println("An error has occurred while mounting LittleFS");
    }
    else
    {
        Serial.println("LittleFS mounted successfully");
    }
}

// Initialize WiFi
void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);
    serverInit();
}

void serverInit()
{
    initWiFi();
    initLittleFS();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.html", "text/html"); });

    server.serveStatic("/", LittleFS, "/");

    server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String(); });

    events.onConnect([](AsyncEventSourceClient *client)
                     {
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000); });
    server.addHandler(&events);

    // Start server
    server.begin();
}

void loop()
{
    serverUpdate();
}

void serverUpdate()
{
    if ((millis() - lastTime) > timerDelay)
    {
        // Send Events to the client with the Sensor Readings Every 10 seconds
        events.send("ping", NULL, millis());
        events.send(getSensorReadings().c_str(), "new_readings", millis());
        lastTime = millis();
    }
}
