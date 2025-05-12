#include <cmath>

#ifndef VECTEUR_H
#define VECTEUR_H
struct Vecteur2
{
    double x;
    double y;

    inline double module2();
    inline double module();
    double getAngle();
    inline Vecteur2 normalize();
};

#endif // VECTEUR_H