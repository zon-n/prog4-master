#include "traction_control.h"

/// @brief 
TractionControl::TractionControl() : steeringPID(&inputAngle, &outputAngle, &setpointAngle, kp_steering, ki_steering, kd_steering, AUTOMATIC)
{
    inputAngle = 0.0;
    setpointAngle = 0.0;
    outputAngle = 0.0;
    steeringPID.SetOutputLimits(-90, 90); // Définit les limites de sortie du PID
    steeringPID.SetSampleTime(100); // Règle la fréquence d'échantillonnage du PID 
}

void TractionControl::update(Vecteur2 driverInput, double yaw)
{
    this->yaw = yaw; // Met à jour l'angle de tangage du véhicule
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

    inputAngle = yaw;
    setpointAngle = input.y; // Le paramètre y du vecteur d'entrée représente l'angle de direction souhaité
    steeringPID.Compute();

    output = {speed, outputAngle}; // La sortie du PID est utilisée pour ajuster la direction du véhicule

    return output; // Retourne le vecteur de sortie qui contient la vitesse et l'angle de direction
}

void TractionControl::enable(bool enable)
{
    enabled = enable;
}