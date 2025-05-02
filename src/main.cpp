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

Input controllerInput;

void setup()
{
    Serial.begin(115200);
    controllerInput.start(0, 1, 2, 3, 4, 5);
}

void loop()
{
}

void getInput(Input input)
{
}