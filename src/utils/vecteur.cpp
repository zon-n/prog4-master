#include "vecteur.h"

inline double Vecteur2::module2()
{
    return x * x + y * y;
}

double Vecteur2::getAngle()
{
    return atan2(y, x);
}

inline Vecteur2 Vecteur2::normalize()
{
    double m = sqrt(module2());
    if (m == 0)
        return {0, 0};
    return {x / m, y / m};
}