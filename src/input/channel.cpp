#include <Arduino.h>
#include "channel.h"

    Channel Channel::initialize(int8_t pin, int8_t minLimit, int8_t maxLimit, int8_t defaultValue)
    {
        this->pin = pin;
        this->minLimit = minLimit;
        this->maxLimit = maxLimit;
        this->defaultValue = defaultValue;
        pinMode(pin, INPUT);

        return *this;
    }

    int8_t Channel::readChannel()
    {
        int8_t chValue = pulseIn(pin, HIGH, 30000);
        if (chValue < 128)
            return defaultValue;
        return map(chValue, 1000, 2000, minLimit, maxLimit);
    }

    bool Channel::readSwitch()
    {
        return (readChannel() > 50);
    }