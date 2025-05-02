#include "vecteur.h"

inline double Vecteur2::module2()
{
    return x * x + y * y;
}

inline double Vecteur2::getAngle()
{
    return atan2(y, x);
}