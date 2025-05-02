#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <LittleFS.h>
#include "serveur.h"

AsyncWebServer server(80);
AsyncEventSource events("/events");

Serveur::Serveur()
{
    initWifi();     // Initialize WiFi
    initLittleFS(); // Initialize LittleFS

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.html", "text/html"); });

    server.serveStatic("/", LittleFS, "/");

    server.on("/events", HTTP_GET, [this](AsyncWebServerRequest *request)
              { request->send(200, "application/json", getTelemetry()); });

    events.onConnect([this](AsyncEventSourceClient *client)
                     {
        if (client->lastId()) {
            Serial.printf("Client reconnected! Last message ID: %u\n", client->lastId());
        }
        client->send("Connected to ESP32", "message", millis()); });
    server.addHandler(&events);

    server.begin(); // Start the server
}

void Serveur::update()
{
    // Update the server
    if (millis() - lastTime > updateInterval)
    {
        events.send("ping", NULL, millis());                // Send a ping event to keep the connection alive
        sendData();// Send telemetry data
        lastTime = millis();
        sendData();
    }
}

void Serveur::sendData()
{
    // Send data to the client
    events.send(getTelemetry(), "telemetry", millis());
}

String Serveur::getTelemetry()
{
    // Get telemetry data
    jsonData["throttle"] = throttle;
    jsonData["steering"] = steering;

    // TODO: Add other telemetry data if needed

    return JSON.stringify(jsonData);
}

void Serveur::updateTelemetry(uint throttle, uint steering)
{
    // Update telemetry data
    this->throttle = throttle;
    this->steering = steering;
}

void Serveur::getIP()
{
    // Get the IP address of the ESP32
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
}

void initLittleFS()
{
    // Initialize LittleFS
    if (!LittleFS.begin())
    {
        Serial.println("Failed to mount file system");
        return;
    }
    Serial.println("LittleFS mounted successfully");
}

void Serveur::initWifi()
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
}