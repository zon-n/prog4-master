#include "imu.h"
#include "filtering.h"

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
    dt = (currentTime - previousTime);
    if (dt >= UPDATE_FREQUENCY)
    {
        read();
        updateAccel();
        updateGyro();
        previousTime = currentTime;
    }
}

void IMU::read()
{
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
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
    int16_t pitchAccelerometer = atan(-ax / sqrt(ay * ay + az * az)) * 180 / M_PI;
    int16_t pitchGyro = pitchGyro + gx * dt / 1000;
    int16_t pitch = filtering(pitchAccelerometer, pitchGyro, rotationTrustA, rotationTrustG);

    return pitch;
}

int16_t IMU::getRoll()
{
    int16_t rollAccelerometer = atan(ay / sqrt(ax * ax + az * az)) * 180 / M_PI;
    int16_t rollGyro = rollGyro + gy * dt / 1000;
    int16_t roll = filtering(rollAccelerometer, rollGyro, rotationTrustA, rotationTrustG);
    return roll;
}

int16_t IMU::getYaw()
{
    int16_t yawAccelerometer = atan(sqrt(ax * ax + ay * ay) / az) * 180 / M_PI;
    int16_t yawGyro = yawGyro + gz * dt / 1000;
    int16_t yaw = filtering(yawAccelerometer, yawGyro, rotationTrustA, rotationTrustG);
    return yaw;
}
