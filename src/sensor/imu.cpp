#include "imu.h"

void IMU::start()
{
    // Initialiser le MPU6050
    mpu.initialize();

    // Régler le décalage des capteurs
    mpu.setXAccelOffset(OFFSETS[0]);
    mpu.setYAccelOffset(OFFSETS[1]);
    mpu.setZAccelOffset(OFFSETS[2]);
    mpu.setXGyroOffset(OFFSETS[3]);
    mpu.setYGyroOffset(OFFSETS[4]);
    mpu.setZGyroOffset(OFFSETS[5]);
}

void IMU::update()
{
    currentTime = millis();
    if (currentTime - previousTime >= UPDATE_FREQUENCY)
    {
        read();
        updateAccel();
        updateGyro();
        updateRotation();
        previousTime = currentTime;
    }
}

void IMU::read()
{
    mpu.getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
}

int16_t IMU::getAccelX()
{
    return ax;
}

int16_t IMU::getAccelY()
{
    return ay;
}

int16_t IMU::getAccelZ()
{
    return az;
}

int16_t IMU::getGyroX()
{
    return gx;
}

int16_t IMU::getGyroY()
{
    return gy;
}

int16_t IMU::getGyroZ()
{
    return gz;
}

int16_t IMU::getPitch()
{
    return atan(-ax / sqrt(ay * ay + az * az)) * 180 / M_PI;
}

int16_t IMU::getRoll()
{
    return atan(ay / sqrt(ax * ax + az * az)) * 180 / M_PI;
}

int16_t IMU::getYaw()
{
    return atan(sqrt(ax * ax + ay * ay) / az) * 180 / M_PI;
}
