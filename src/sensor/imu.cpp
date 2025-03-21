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

void IMU::read()
{
    mpu.getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
}

int16_t IMU::getAccelX()
{
    return accelX;
}

int16_t IMU::getAccelY()
{
    return accelY;
}

int16_t IMU::getAccelZ()
{
    return accelZ;
}

int16_t IMU::getGyroX()
{
    return gyroX;
}

int16_t IMU::getGyroY()
{
    return gyroY;
}

int16_t IMU::getGyroZ()
{
    return gyroZ;
}
