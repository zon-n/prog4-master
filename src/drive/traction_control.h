#include <Arduino.h>
#include "PID_v1.h"
#include "utils/vecteur.h"

#pragma once
class TractionControl
{
public:
    double kp_steering = 1.0, ki_steering = 0.0, kd_steering = 0.0;
    PID steeringPID;
    bool enabled = true;

    double yaw; // Angle de tangage du véhicule

    Vecteur2 input;
    Vecteur2 output;

    double speed; // Vitesse du véhicule entre -127 et 128

    double inputAngle;
    double setpointAngle;
    double outputAngle;

    /**
     * @brief Constructeur de la classe TractionControl
     * @details Initialise le PID pour le contrôle de la direction
     * @param kp_steering Coefficient de proportionnalité
     * @param ki_steering Coefficient d'intégration
     * @param kd_steering Coefficient de dérivation
     */
    TractionControl();

    /**
     * @brief Mise à jour des données du contrôle de traction
     * @param driverInput Vecteur2 contenant la vitesse et l'angle de direction
     * @param pitch Angle de tangage du véhicule
     */
    void update(Vecteur2 driverInput, double pitch);

    /**
     * @brief Contrôle de la direction du véhicule par l'intermédiaire du PID
     * @return Vecteur2 contenant la vitesse et l'angle de direction
     */
    Vecteur2 steeringControl();

    /**
     * @brief Active ou désactive le contrôle de traction
     */
    void enable(bool enable);
};
