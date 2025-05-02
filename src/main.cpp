#include <Arduino.h>
#include "input/input.h"
#include "drive/traction_control.h"
#include "sensor/imu.h"

#define THROTTLE_PIN 9
#define STEERING_PIN 10
#define FREQUENCY 50 // Fréquence PWM en Hz

#define THROTTLE_CHANNEL 3
#define STEERING_CHANNEL 4

#define THROTTLE_MIN_PWM 1000
#define THROTTLE_MAX_PWM 2000

#define STEERING_MIN_PWM 1000
#define STEERING_MAX_PWM 2000

Input controllerInput;
TractionControl tractionControl;
IMU imu;

void setup()
{
    Serial.begin(115200);

    controllerInput.start(0, 1, 2, 3, 4, 5); // TODO : Set the correct pins for the channels
    Serial.println("Input channels initialized.");

    tractionControl.start();
    Serial.println("Traction control initialized.");

    imu.start();
    Serial.println("IMU initialized.");

    //TODO: Initialize telemetry server
}

void loop()
{
    int8_t throttle = controllerInput.getChannel(THROTTLE_CHANNEL);
    int8_t steering = controllerInput.getChannel(STEERING_CHANNEL);
    
    // Execute the traction control algorithm

    // Update and read the IMU data
    imu.update();
    Vecteur2 measuredAccel = {imu.getAccelX(), imu.getAccelY()};

    // Traction control logic 
    tractionControl.update(measuredAccel, throttle, steering);
    tractionControl.tractionControl(&throttle, &steering);

    // Ouput telemetry to server 
    // TODO: Implement telemetry output 

    // Output the throttle and steering values to the motors
    drive(throttle, steering);
}

void drive(int8_t throttle, int8_t steering)
{
    analogWriteFrequency(FREQUENCY); // Set the PWM frequency to 50Hz
    analogWrite(THROTTLE_PIN, map(throttle, -128, 127, THROTTLE_MIN_PWM, THROTTLE_MAX_PWM)); // Map the throttle value to the PWM range
    analogWrite(STEERING_PIN, map(steering, -128, 127, STEERING_MIN_PWM, STEERING_MAX_PWM)); // Map the steering value to the PWM range
}


