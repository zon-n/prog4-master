#include <Arduino.h>
#include <PID_v1.h>
#include <Wire.h>
#include "utils/vecteur.h"

class TractionControl
{
private:
    Vecteur2 lastCarbehaviour = {0, 0};
    double maxDeltaX = 5; // TODO: Changeme
    double maxDeltaY = 5; // TODO: Changeme

    const double KP_DIRECTION = 0;
    const double KI_DIRECTION = 0;
    const double KD_DIRECTION = 0;

    const double KP_THROTTLE = 0;
    const double KI_THROTTLE = 0;
    const double KD_THROTTLE = 0;

    
    const double KP_DIRECTIONv2 = 0;
    const double KI_DIRECTIONv2 = 0;
    const double KD_DIRECTIONv2 = 0;

    const double KP_THROTTLEv2 = 0;
    const double KI_THROTTLEv2 = 0;
    const double KD_THROTTLEv2 = 0;

    double input, output, setpoint;
    double kp, ki, kd;
    PID pid;

public:
    void start();

    Vecteur2 adjustDirection(Vecteur2 carBehaviour, Vecteur2 expectedBehaviour);
    Vecteur2 throttleControl(Vecteur2 carBehaviour);
    Vecteur2 turnSpeedControl(Vecteur2 carBehaviour);
};
