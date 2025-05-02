#include <Arduino.h>
#include <web/serveur.h> // Include the Serveur class header    

Serveur serveur; // Create an instance of the Serveur class

void setup()
{
    Serial.begin(115200); // Initialize serial communication
    serveur.getIP();       // Get the IP address of the ESP32
    delay(1000);           // Delay to allow for WiFi connection
}

void loop()
{
    serveur.update(); // Update the server
    delay(100);        // Delay to avoid overwhelming the server
    // Read the throttle and steering values from the PWM channels
    uint throttle = random(0, 10); // Simulate throttle input
    uint steering = random(0, 10); // Simulate steering input

    serveur.updateTelemetry(throttle, steering); // Update telemetry data
}