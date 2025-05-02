#include <Arduino.h>

class Drive {
    const int RAYON_ROUE = 64;
    const int DISTANCE_ROUES = 300;
    const int maxSpeedKmH = 30;
    const int maxSpeedMs = maxSpeedKmH * 1000 / 3600; 

    int8_t getThrottle(int8_t throttle, int8_t steering);
    int8_t getSteering(int8_t throttle, int8_t steering);
};

