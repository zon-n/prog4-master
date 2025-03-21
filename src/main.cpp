#include <Arduino.h>
#include "input/input.h"

#define THROTTLE_PIN 9
#define STEERING_PIN 10

#define THROTTLE_CHANNEL 3
#define STEERING_CHANNEL 4

#define THROTTLE_MIN_PWM 1000
#define THROTTLE_MAX_PWM 2000

#define STEERING_MIN_PWM 1000
#define STEERING_MAX_PWM 2000

Input input;

void setup()
{
    Serial.begin(115200);
    input.start(0, 1, 2, 3, 4, 5);
}

void loop()
{
    int8_t throttle = input.getChannel(THROTTLE_CHANNEL);
    int8_t steering = input.getChannel(STEERING_CHANNEL);
    int throttleValue = map(throttle, -128, 127, THROTTLE_MIN_PWM, THROTTLE_MAX_PWM);
    int steeringValue = map(steering, -128, 127, STEERING_MIN_PWM, STEERING_MAX_PWM);
    Serial.println(throttleValue);
    Serial.println(steeringValue);
}

void drive(Input input)
{
    int8_t throttle = input.getChannel(THROTTLE_CHANNEL);
    int8_t steering = input.getChannel(STEERING_CHANNEL);

    int throttleValue = map(throttle, -128, 127, THROTTLE_MIN_PWM, THROTTLE_MAX_PWM);
    int steeringValue = map(steering, -128, 127, STEERING_MIN_PWM, STEERING_MAX_PWM);

    // TODO: Retourner valeurs sur les moteurs
}