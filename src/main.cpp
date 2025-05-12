#include <Arduino.h>
#include <Wire.h>
#include <MPU9250.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <Arduino_JSON.h>
#include "drive/traction_control.h"
#include <utils/vecteur.h>

#define CRC_I2C_ADDRESS 0x02
#define MPU_I2C_ADDRESS 0x68
#define UPDATE_DELAY 100

// Replace with your network credentials
const char *ssid = "DESKTOP-MC5O0GI 6923";
const char *password = "22]j8M81";

AsyncWebServer server(80);
AsyncEventSource events("/events");
JSONVar readings;

unsigned long lastTime = 0;
unsigned long timerDelay = 100;

MPU9250 mpu;
TractionControl tractionControl;
Vecteur2 driverInput;
Vecteur2 correctedDriverInput = driverInput;

// TODO: Changeme
int8_t speed = 50;
int8_t orientation = 50;

String getSensorReadings()
{
    readings["input_x"] = driverInput.x;
    readings["input_y"] = driverInput.y;
    readings["output_x"] = correctedDriverInput.x;
    readings["output_y"] = correctedDriverInput.y;
    readings["kp"] = tractionControl.kp_steering;
    readings["ki"] = tractionControl.ki_steering;
    readings["kd"] = tractionControl.kd_steering;

    String jsonString = JSON.stringify(readings);
    return jsonString;
}

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

void serverInit()
{
    initWiFi();
    initLittleFS();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.html", "text/html"); });

    server.serveStatic("/", LittleFS, "/");

    server.on("/changeKP", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  double testParam = request->getParam("value")->value().toDouble();
                  request->send(LittleFS, "/index.html", "text/html");
                  tractionControl.kp_steering += testParam;
                  Serial.println("KP changed to: " + String(tractionControl.kp_steering));
                  });
    server.on("/changeKI", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  double testParam = request->getParam("value")->value().toDouble();
                  request->send(LittleFS, "/index.html", "text/html");
                  tractionControl.ki_steering += testParam;
                  Serial.println("KI changed to: " + String(tractionControl.ki_steering));
                  });
    server.on("/changeKD", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  double testParam = request->getParam("value")->value().toDouble();
                  request->send(LittleFS, "/index.html", "text/html");
                  tractionControl.kd_steering += testParam;
                  Serial.println("KD changed to: " + String(tractionControl.kd_steering));
                  });

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

    client->send("hello!", NULL, millis(), 10000); });
    server.addHandler(&events);

    server.begin();
}

void serverUpdate()
{
    if ((millis() - lastTime) > timerDelay)
    {
        events.send("ping", NULL, millis());
        events.send(getSensorReadings().c_str(), "new_readings", millis());
        lastTime = millis();
    }
}

Vecteur2 getInput()
{
    // int8_t speed;
    // int8_t orientation;

    speed = constrain(speed + random(-5, 5), -127, 127);
    orientation = constrain(orientation + random(-10, 10), -90, 90);

    // Wire.requestFrom(CRC_I2C_ADDRESS, 2);
    // while (Wire.available() >= 2)
    // {
    //     speed = Wire.read();m
    //     Wire.available();
    //     orientation = Wire.read();
    //     //Serial.println(speed);
    //     Serial.println(orientation);
    // }

    return {speed, orientation};
}

void sendCorrectedInput(Vecteur2 correctedDriverInput)
{
    int8_t speed = static_cast<int8_t>(correctedDriverInput.x);
    int8_t orientation = static_cast<int8_t>(correctedDriverInput.y);

    Wire.beginTransmission(CRC_I2C_ADDRESS);
    Wire.write(speed);
    Wire.write(orientation);
    Wire.endTransmission();
}

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    serverInit();
    // if (!mpu.setup(MPU_I2C_ADDRESS))
    // {
    //      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
    //      while (1)
    //      {
    //          delay(1000);
    //      }
    //  }
    //  else
    //  {
    //      Serial.println("MPU connection successful. Start calibration");
    //      mpu.calibrateAccelGyro();
    //      mpu.calibrateMag();
    //      Serial.println("Calibration successful");
    //  }
    tractionControl.enable(false);
}

void loop()
{
    // Serial.println("looping...");
    driverInput = getInput();

    // Serial.print("MPU status:");
    // Serial.println(mpu.isConnected() ? "connected" : "disconnected");

    // if (mpu.update())
    // {
    //     Serial.println("MPU update successful");
    //     static uint32_t prev_ms = millis();
    //     if (millis() > prev_ms + UPDATE_DELAY)
    //     {
    //         prev_ms = millis();
    //     }
    // }
    // else
    // {
    //     Serial.println("MPU update failed");
    // }

    // Serial.print("Orientation: ");
    // Serial.println(mpu.getYaw());

    // tractionControl.update(driverInput, mpu.getYaw());

    // TODO Changeme
    tractionControl.update(driverInput, 0);

    correctedDriverInput = tractionControl.steeringControl();

    // Serial.println("Sending corrected driver input ....");
    // // sendCorrectedInput(correctedDriverInput);
    // Serial.println("Corrected driver input sent");

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi connection failed");
        initWiFi();
    }

    if (millis() - lastTime > timerDelay)
    {
        serverUpdate();
    }

    // Serial.println("Driver input: " + String(driverInput.x) + " " + String(driverInput.y));
    // Serial.println("Corrected driver input: " + String(correctedDriverInput.x) + " " + String(correctedDriverInput.y));
    // Serial.println("--------------------------------------------------");
}
