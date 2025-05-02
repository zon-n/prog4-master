#include "vecteur.h"
#include <cmath>

inline double Vecteur2::module2()
{
    return x * x + y * y;
}

double Vecteur2::getAngle()
{
    return atan2(y, x);
}