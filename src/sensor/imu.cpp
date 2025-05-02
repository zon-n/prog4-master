#include "imu.h"
#include "filtering.h"

void IMU::start()
{
    if (!mpu.begin())
    {
        Serial.println("MPU6050 mounting error");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("MPU6050 mounted");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange())
    {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange())
    {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth())
    {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }

    Serial.println("");
    delay(100);
}

void IMU::update()
{
    currentTime = millis();
    dt = (currentTime - previousTime);
    if (dt >= UPDATE_FREQUENCY)
    {
        /* Get new sensor events with the readings */
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        ax = a.acceleration.x - OFFSETS[0];
        ay = a.acceleration.y - OFFSETS[1];
        az = a.acceleration.z - OFFSETS[2];
        gx = g.gyro.x - OFFSETS[3];
        gy = g.gyro.y - OFFSETS[4];
        gz = g.gyro.z - OFFSETS[5];

        previousTime = currentTime;
    }
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
