#include "traction_control.h"

void TractionControl::start()
{
    PID pidDirectionX(&inputDirectionX, &outputDirectionX, &setpointDirectionX, KP_DIRECTION_X, KD_DIRECTION_X, KI_DIRECTION_X, DIRECT);
    PID pidDirectionY(&inputDirectionY, &outputDirectionY, &setpointDirectionY, KP_DIRECTION_Y, KD_DIRECTION_Y, KI_DIRECTION_Y, DIRECT);
    PID pidThrottle(&inputThrottle, &outputThrottle, &setpointThrottle, KP_THROTTLE, KD_THROTTLE, KI_THROTTLE, DIRECT);
    PID pidTurnSpeed(&inputTurnSpeed, &outputTurnSpeed, &setpointTurnSpeed, KP_TURN_SPEED, KD_TURN_SPEED, KI_TURN_SPEED, DIRECT);
}

Vecteur2 TractionControl::adjustDirection(Vecteur2 carBehaviour, Vecteur2 expectedBehaviour)
{
    // calculer erreur sur x
    inputDirectionX = carBehaviour.x;
    setpointDirectionX = expectedBehaviour.x;
    pidDirectionX.Compute();

    // calculer erreur sur y
    inputDirectionY = carBehaviour.y;
    setpointDirectionY = expectedBehaviour.y;

    pidDirectionY.Compute();

    Vecteur2 error = {outputDirectionX, outputDirectionY};
    return error;
}

Vecteur2 TractionControl::throttleControl(Vecteur2 carBehaviour)
{   

}

Vecteur2 TractionControl::turnSpeedControl(Vecteur2 carBehaviour)
{

}
