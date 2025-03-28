#include "traction_control.h"

void TractionControl::start()
{
    PID pid(&input, &output, &setpoint, kd, ki, kd, DIRECT);
}

Vecteur2 TractionControl::adjustDirection(Vecteur2 carBehaviour, Vecteur2 expectedBehaviour)
{
    // calculer erreur sur x
    input = carBehaviour.x;
    setpoint = expectedBehaviour.x;
    pid.SetTunings(KP_DIRECTION, KD_DIRECTION, KI_DIRECTION);

    pid.Compute();
    double errorX = output;

    // calculer erreur sur y
    input = carBehaviour.y;
    setpoint = expectedBehaviour.y;
    pid.SetTunings(KP_THROTTLE, KD_THROTTLE, KI_THROTTLE);

    pid.Compute();
    double errorY = output;

    Vecteur2 error = {errorX, errorY};
    return error;
}

Vecteur2 TractionControl::throttleControl(Vecteur2 carBehaviour)
{   /*
    double throttleOutput = 0;
    double targetThrottle = 255;

    // Regarder si il y a un top grand changement d'accel
    // if (abs(carBehaviour.x - lastCarbehaviour.x) > maxDeltaX)
    // {
    //     double adjustedX = constrain(abs(carBehaviour.x - lastCarbehaviour.x), 0, maxDeltaX);
    // }
    // else

    bool derapage = (abs(carBehaviour.x) > maxDeltaX)

        if (derapage)
    {
        targetThrottle = throttleOutput * 0.5;
    }

    input = throttleOutput;
    setpoint = targetThrottle;

    pid.SetTunings(KP_DIRECTIONv2, KD_DIRECTIONv2, KI_DIRECTIONv2);
    pid.Compute();

    int throttle = constrain(throttleOutput + output, 0, 255);

    return throttle;

    double adjustedY = constrain(abs(carBehaviour.x - lastCarbehaviour.x), 0, maxDeltaY);*/
}

Vecteur2 TractionControl::turnSpeedControl(Vecteur2 carBehaviour)
{
    // Ajuster la vitesse pendant les virages
}
