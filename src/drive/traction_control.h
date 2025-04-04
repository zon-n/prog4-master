#include <Arduino.h>
#include <PID_v1.h>
#include <Wire.h>
#include "utils/vecteur.h"

class TractionControl
{
private:
    Vecteur2 lastCarbehaviour = {0, 0};

    const double MAX_DELTA_YAW = 0.5; // maximum yaw rate in rad/s
    const double MAX_DELTA_THROTTLE = 0.5; // maximum throttle change in m/s^2

    const double KP_DIRECTION_X = 0;
    const double KI_DIRECTION_X = 0;
    const double KD_DIRECTION_X = 0;

    const double KP_DIRECTION_Y = 0;
    const double KI_DIRECTION_Y = 0;
    const double KD_DIRECTION_Y = 0;

    const double KP_THROTTLE = 0;
    const double KI_THROTTLE = 0;
    const double KD_THROTTLE = 0;

    const double KP_TURN_SPEED = 0;
    const double KI_TURN_SPEED = 0;
    const double KD_TURN_SPEED = 0;

    PID pidDirectionX;
    PID pidDirectionY;
    PID pidThrottle;
    PID pidTurnSpeed;

    double inputDirectionX, outputDirectionX, setpointDirectionX;
    double inputDirectionY, outputDirectionY, setpointDirectionY;
    double inputThrottle, outputThrottle, setpointThrottle;
    double inputTurnSpeed, outputTurnSpeed, setpointTurnSpeed;

public:
    void start();

    Vecteur2 adjustDirection(Vecteur2 carBehaviour, Vecteur2 expectedBehaviour);
    Vecteur2 throttleControl(Vecteur2 carBehaviour);
    Vecteur2 turnSpeedControl(Vecteur2 carBehaviour);
};
