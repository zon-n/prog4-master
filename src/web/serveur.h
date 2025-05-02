#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <LittleFS.h>


class Serveur
{
private:
    // Private members
    const char *ssid = "SSID";
    const char *password = "PASSWORD";

    JSONVar jsonData;
    ulong lastTime = 0;
    ulong updateInterval = 10;

    uint throttle = 0;
    uint steering = 0;

public:
    // Public members
    Serveur();
    void update();
    void sendData();
    void getIP();
    void initWifi();
    void initLittleFS();
    String getTelemetry();
    void updateTelemetry(uint throttle, uint steering);
};
