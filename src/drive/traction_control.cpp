#include "traction_control.h"

TractionControl::TractionControl() : steeringPID(&inputAngle, &ouputAngle, &setpointAngle, kp_steering, ki_steering, kd_steering, AUTOMATIC)
{
    // Constructor initializes the PID controller with the specified parameters
    inputAngle = 0.0;
    setpointAngle = 0.0;
    ouputAngle = 0.0;
}

void TractionControl::update(Vecteur2 driverInput, double pitch)
{
    this->pitch = pitch; // Update the pitch angle of the vehicle
    input = driverInput;
    speed = input.x; // Le paramètre x du vecteur d'entrée représente la vitesse du véhicule

    steeringControl();
}

Vecteur2 TractionControl::steeringControl()
{
    if (!enabled)
    {
        return {input.x + random(-5, 5), input.y + random(-5, 5)}; // Si le contrôle est désactivé, retourne l'entrée d'origine
    }

    inputAngle = pitch;
    setpointAngle = input.y; // Le paramètre y du vecteur d'entrée représente l'angle de direction souhaité
    steeringPID.Compute();

    output = {speed, ouputAngle}; // La sortie du PID est utilisée pour ajuster la direction du véhicule

    return output; // Retourne le vecteur de sortie qui contient la vitesse et l'angle de direction
}

void TractionControl::enable(bool enable)
{
    enabled = enable;
}