#include "traction_control.h"

void TractionControl::start()
{
    PID PIDSteering(&inputSteering, &outputSteering, &setpointSteering, KP_STEERING, KI_STEERING, KD_STEERING, DIRECT);
}

void TractionControl::update(Vecteur2 measuredAccel, int8_t throttle, int8_t steering)
{
    this-> inputThrottle = throttle;
    this-> inputSteering = steering;
    this->carBehaviour = measuredAccel;
    this-> inputSteering = steering;
}

void TractionControl::tractionControl(int8_t* throttle, int8_t* steering)
{
    adjustSteering();
    adjustInputDelta();

    // Convert to int8_t for the output
    *throttle = int8_t(outputThrottle); 
    *steering = int8_t(outputSteering);
}

void TractionControl::adjustSteering()
{
    this->inputSteering = map(carBehaviour.getAngle(), -90, 90, -128, 127);
    this->setpointSteering = inputSteering;
    pidSteering.Compute();
}

/**
 * @brief Adjust the input throttle and steering to avoid sudden changes.
 */

void TractionControl::adjustInputDelta()
{
    if (inputThrottle - lastThrottle > MAX_DELTA_THROTTLE)
    {
        outputThrottle = lastThrottle + MAX_DELTA_THROTTLE;
    }
    else if(inputThrottle - lastThrottle < -MAX_DELTA_THROTTLE)
    {
        outputThrottle = lastThrottle - MAX_DELTA_THROTTLE;
    }

    if(outputSteering - lastSteering > MAX_DELTA_STEERING)
    {
        outputSteering = lastSteering + MAX_DELTA_STEERING;
    }
    else if(outputSteering - lastSteering < -MAX_DELTA_STEERING)
    {
        outputSteering = lastSteering - MAX_DELTA_STEERING;
    }
}







