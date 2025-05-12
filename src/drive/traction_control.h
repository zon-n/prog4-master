#include <Arduino.h>
#include "PID_v1.h"
#include "utils/vecteur.h"

class TractionControl
{
public:
    double kp_steering = 0.5, ki_steering = 0.0, kd_steering = 0.0;
    PID steeringPID;
    bool enabled = true;

    double pitch; // Angle de tangage du véhicule

    Vecteur2 input;
    Vecteur2 output;

    double speed; // Vitesse du véhicule entre -127 et 128

    double inputAngle;
    double setpointAngle;
    double ouputAngle;

    TractionControl();
    void update(Vecteur2 driverInput, double pitch);
    Vecteur2 steeringControl();
    void enable(bool enable);
};
