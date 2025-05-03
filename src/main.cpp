#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <Arduino_JSON.h>

// Replace with your network credentials
const char *ssid = "SSID";
const char *password = "PASSWORD";

AsyncWebServer server(80);
AsyncEventSource events("/events");
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 100;

int starting1, starting2, starting3, starting4;

// Get Sensor Readings and return JSON object
String getSensorReadings()
{
    readings["input_x"] = starting1;
    readings["input_y"] = starting2;
    readings["output_x"] = starting1 + random(-5, 5);
    readings["output_y"] = starting2 + random(-5, 5);

    starting1 = starting1 > 100 ? 0 : starting1 + random(0, 5);
    starting2 = starting2 > 100 ? 0 : starting2 + random(0, 5);

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

void setup()
{
    Serial.begin(115200);
    serverInit();
    starting1 = random(-50, 50);
    starting2 = random(-50, 50);
    starting3 = random(-50, 50);
    starting4 = random(-50, 50);
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        initWiFi();
    }

    if (millis() - lastTime > timerDelay)
    {
        serverUpdate();
    }
}


