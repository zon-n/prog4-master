#include <Arduino.h>
#include <PID_v1.h>
#include <Wire.h>
#include "utils/vecteur.h"

class TractionControl
{
private:
    Vecteur2 carBehaviour;
    Vecteur2 lastCarBehaviour;

    double inputSteering, outputSteering, setpointSteering;
    double inputThrottle, outputThrottle;

    int8_t lastThrottle = 0;
    int8_t lastSteering = 0;

    const double MAX_DELTA_THROTTLE = 0.5; // maximum throttle change between -128 and 127
    const double MAX_DELTA_STEERING = 0.5; // maximum steering change between -128 and 127

    const double KP_STEERING = 0;
    const double KI_STEERING = 0;
    const double KD_STEERING = 0;

    PID pidSteering;

public:
    void start();
    void update(Vecteur2 measuredAccel, int8_t inputThrottle, int8_t inputSteering);
    void tractionControl(int8_t* throttle, int8_t* steering);

    void adjustSteering();
    void adjustInputDelta();
};
