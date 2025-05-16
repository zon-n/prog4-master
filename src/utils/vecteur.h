#include <cmath>

#pragma once

/**
 * @brief Vecteur2 est une structure représentant un vecteur à deux dimensions
 */
struct Vecteur2
{
    double x;
    double y;

    /**
     * @brief calcule le module au carré du vecteur pour éviter la racine carrée
     */
    inline double module2();

    /**
     * @brief calcule l'angle du vecteur
     */
    double getAngle();

    /**
     * @brief normalise le vecteur
     */
    inline Vecteur2 normalize();
};
