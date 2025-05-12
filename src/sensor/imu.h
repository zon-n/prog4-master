#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

struct IMU
{
    static constexpr int8_t GLOBAL_RAD = 175;

    static constexpr int16_t OFFSETS[6] = {0, 0, 0, 0, 0, 0};
    static constexpr int8_t SDA = 9;
    static constexpr int8_t SCL = 10;
    static constexpr int8_t UPDATE_FREQUENCY = 100;

    static const int8_t rotationTrustA = 2;
    static const int8_t rotationTrustG = 8;

    public:
        void start();
        void update();
        void read();

        int16_t getAccelX();
        int16_t getAccelY();
        int16_t getAccelZ();

        int16_t getGyroX();
        int16_t getGyroY();
        int16_t getGyroZ();

        int16_t getPitch();
        int16_t getRoll();
        int16_t getYaw();

    private:
        Adafruit_MPU6050 mpu;
        unsigned long previousTime;
        unsigned long currentTime;
        unsigned long dt;

        int16_t ax;
        int16_t ay;
        int16_t az;

        int16_t gx;
        int16_t gy;
        int16_t gz;

        int16_t pitch;
        int16_t roll;
        int16_t yaw;

        int16_t lastAccelX;
        int16_t lastAccelY;
        int16_t lastAccelZ;

        int16_t lastGyroX;
        int16_t lastGyroY;
        int16_t lastGyroZ;
};