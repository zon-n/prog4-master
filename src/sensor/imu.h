#include <Arduino.h>
#include "I2Cdev.h"
#include "MPU6050.h"

struct IMU
{
    static constexpr int16_t OFFSETS[6] = {0, 0, 0, 0, 0, 0};
    static constexpr int8_t SDA = 9;
    static constexpr int8_t SCL = 10;

    public:
        void start();
        void read();
        int16_t getAccelX();
        int16_t getAccelY();
        int16_t getAccelZ();
        int16_t getGyroX();
        int16_t getGyroY();
        int16_t getGyroZ();

    private:
        MPU6050 mpu;
        int16_t accelX;
        int16_t accelY;
        int16_t accelZ;
        int16_t gyroX;
        int16_t gyroY;
        int16_t gyroZ;
};