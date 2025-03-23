#include <Arduino.h>
#include "traction_control.h"
#include <PID_v1.h>
#include <Wire.h>
#define THROTTLE_PIN 9 //Voir si on dois passer cela en argument

double setpoint = 0;
double input, output;

double Kp = 2, Ki = 5, Kd = 1; //A ajuster

PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

    void TractionControl::adjustStraightLine(int ax, int ay, int az, int rx, int ry, int rz) {

        input = ry * 180 / M_PI;

        pid.Compute();

        int baselinePuissance = 100;//A ajuster

        //Verifier si on doit inverser les valeurs ou inverser les signes +/-, ou juste garder une correction que plutot deux.
        int correctionPuissance = constrain(baselinePuissance + output, 0, 255); //constrain pour garder la valeur entre 0 et 255
        int correctionPuissanceOppose = constrain(baselinePuissance - output, 0, 255);

        analogWrite(THROTTLE_PIN, correctionPuissance);
        analogWrite(THROTTLE_PIN, correctionPuissanceOppose);
    }

    void TractionControl::turnAssist(int ax, int ay, int az, int rx, int ry, int rz) {
        // TODO: Implement this
    }

    void TractionControl::throttleControl(int ax, int ay, int az, int rx, int ry, int rz) {
        // TODO: Implement this
    }

    void TractionControl::controleDerapage(int ax, int ay, int az, int rx, int ry, int rz) {
        // TODO: Implement this
    }