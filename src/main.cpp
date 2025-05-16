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
const char *ssid = "SSID";
const char *password = "PASSWORD";

// Création d'un serveur web asynchrone sur le port 80
AsyncWebServer server(80);
AsyncEventSource events("/events"); // Création d'une source d'événements asynchrone pour le serveur
JSONVar readings; // Objet JSON pour stocker les lectures des capteurs

unsigned long lastTime = 0;
unsigned long timerDelay = 100;

// Instances nécessaires pour le système de contrôle de traction	
MPU9250 mpu;
TractionControl tractionControl;
Vecteur2 driverInput;
Vecteur2 correctedDriverInput = driverInput;

int8_t speed;
int8_t orientation;

/**
 * @brief Fonction pour obtenir les lectures des capteurs
 * @return Renvoie une chaîne JSON contenant les lectures des capteurs
 */
String getSensorReadings()
{
    readings["input_speed"] = driverInput.x;
    readings["input_orientation"] = driverInput.y;
    readings["output_speed"] = correctedDriverInput.x;
    readings["output_orientation"] = correctedDriverInput.y;
    readings["kp"] = tractionControl.kp_steering;
    readings["ki"] = tractionControl.ki_steering;
    readings["kd"] = tractionControl.kd_steering;

    String jsonString = JSON.stringify(readings);
    return jsonString;
}

/**
 * @brief Initialiser le système de fichiers LittleFS utilisé par ESP32 pour stocker des fichiers du serveur web
 */
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

/**
 * @brief Initialiser la connexion WiFi
 */
void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");

    // Attendre la connexion au WiFi
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

/**
 * @brief Initialiser le serveur web
 */
void serverInit()
{
    initWiFi();
    initLittleFS();

    // Envoie la page HTML index.html lorsque la racine est demandée
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.html", "text/html"); });

    server.serveStatic("/", LittleFS, "/");

    // Renvoie un changement de valeur pour les paramètres PID lorsque la page est demandée
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


    // Renvoie les lectures des capteurs au format JSON par requête http
    server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String(); });

    // Envoie sur la console lorsque le client se reconnecte
    events.onConnect([](AsyncEventSourceClient *client)
                     {
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }

    // Envoie sur la console que le client s'est connecté
    client->send("hello!", NULL, millis(), 10000); });
    server.addHandler(&events);

    server.begin();
}

/**
 * @brief Met à jour le serveur en envoyant les lectures des capteurs
 * @note Envoie un message "ping" toutes les 100ms
 * @note Envoie les lectures des capteurs au format JSON
 */
void serverUpdate()
{
    if ((millis() - lastTime) > timerDelay)
    {
        events.send("ping", NULL, millis());
        events.send(getSensorReadings().c_str(), "new_readings", millis());
        lastTime = millis();
    }
}
/**
 * @brief Fonction pour obtenir les entrées du conducteur via I2C du slave
 * @return Renvoie un vecteur contenant la vitesse et l'orientation
 */
Vecteur2 getInput()
{
    int8_t speed;
    int8_t orientation;

    // Demande de lecture de 2 octets à l'adresse I2C du slave
    Wire.requestFrom(CRC_I2C_ADDRESS, 2);
    while (Wire.available() >= 2)
    {
        speed = Wire.read();
        Wire.available();
        orientation = Wire.read();
    }

    return {speed, orientation};
}

/**
 * @brief Fonction pour envoyer les entrées corrigées au slave via I2C
 * @param correctedDriverInput Vecteur2 contenant la vitesse et l'orientation corrigées
 */
void sendCorrectedInput(Vecteur2 correctedDriverInput)
{
    // Cast la vitesse et l'orientation corrigées en int8_t car le traitement PID est fait en double
    int8_t speed = static_cast<int8_t>(correctedDriverInput.x); 
    int8_t orientation = static_cast<int8_t>(correctedDriverInput.y);

    // Envoie de la vitesse et de l'orientation corrigées au slave via I2C
    Wire.beginTransmission(CRC_I2C_ADDRESS);
    Wire.write(speed);
    Wire.write(orientation);
    Wire.endTransmission();
}

/**
 * @brief Fonction de configuration initiale, éxécutée une fois au démarrage
 * @details Initialise la connexion série, le bus I2C et le serveur web
 */
void setup()
{
    Serial.begin(115200);
    Wire.begin();
    serverInit();

    // Initialisation de la communication I2C avec le MPU9250 et calibration du module
    if (!mpu.setup(MPU_I2C_ADDRESS))
    {
         Serial.println("MPU connection failed");
         while (1)
         {
             delay(1000);
         }
     }
     else
     {
         Serial.println("MPU connection successful. Start calibration");
         mpu.calibrateAccelGyro();
         mpu.calibrateMag();
         Serial.println("Calibration successful");
     }
    tractionControl.enable(false);
}

/**
 * @brief Boucle principale, éxécutée en continu
 * @details Lit les entrées du conducteur, met à jour le MPU9250, met à jour le contrôle de traction et envoie les entrées corrigées au slave
 * @note Envoie les lectures des capteurs au format JSON
 */
void loop()
{
    driverInput = getInput();

    Serial.print("MPU status:");
    Serial.println(mpu.isConnected() ? "connected" : "disconnected");

    // Mise à jour du MPU9250
    if (mpu.update())
    {
        Serial.println("MPU update successful");
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + UPDATE_DELAY)
        {
            prev_ms = millis();
        }
    }
    else
    {
        Serial.println("MPU update failed");
    }

    Serial.print("Orientation: ");
    Serial.println(mpu.getYaw());

    // Mettre à jour le contrôle de traction avec les entrées du conducteur et l'angle de tangage
    tractionControl.update(driverInput, mpu.getYaw());

    // Corrige les entrées du conducteur avec le PID une fois le contrôle de traction mis à jour
    correctedDriverInput = tractionControl.steeringControl();
    sendCorrectedInput(correctedDriverInput);

    // Debug sur la console
    Serial.print("Driver input: ");
    Serial.print(driverInput.x);
    Serial.print(" ");
    Serial.print(driverInput.y);
    Serial.print(" Corrected driver input: ");
    Serial.print(correctedDriverInput.x);
    Serial.print(" ");
    Serial.print(correctedDriverInput.y);

    // Vérifie la connexion WiFi et redémarre si la connexion échoue
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi connection failed");
        initWiFi();
    }

    if (millis() - lastTime > timerDelay)
    {
        serverUpdate();
    }
}
