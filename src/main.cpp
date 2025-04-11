#include <Arduino.h>
#include "sensor/imu.h"

IMU imu;

void setup()
{
    Serial.begin(115200);
    imu.start();
    Serial.println("IMU started");
}

void loop()
{
    imu.update();
    Serial.print("Accelerometer: ");
    Serial.print(imu.getAccelX());
    Serial.print(", ");
    Serial.print(imu.getAccelY());
    Serial.print(", ");
    Serial.println(imu.getAccelZ());

    Serial.print("Gyroscope: ");
    Serial.print(imu.getGyroX());
    Serial.print(", ");
    Serial.print(imu.getGyroY());
    Serial.print(", ");
    Serial.println(imu.getGyroZ());

    delay(1000);
}
