#include <Arduino.h>
#include "input.h"

void Input::start(int8_t ch1Pin, int8_t ch2Pin, int8_t ch3Pin, int8_t ch4Pin, int8_t ch5Pin, int8_t ch6Pin)
{
    channels[0].initialize(ch1Pin, MIN_CANAL[0], MAX_CANAL[0], DEFAULT_CANAL[0]);
    channels[1].initialize(ch2Pin, MIN_CANAL[1], MAX_CANAL[1], DEFAULT_CANAL[1]);
    channels[2].initialize(ch3Pin, MIN_CANAL[2], MAX_CANAL[2], DEFAULT_CANAL[2]);
    channels[3].initialize(ch4Pin, MIN_CANAL[3], MAX_CANAL[3], DEFAULT_CANAL[3]);
    channels[4].initialize(ch5Pin, MIN_CANAL[4], MAX_CANAL[4], DEFAULT_CANAL[4]);
    channels[5].initialize(ch6Pin, MIN_CANAL[5], MAX_CANAL[5], DEFAULT_CANAL[5]);
}

int8_t Input::getChannel(u_int8_t channel)
{
    return channels[channel - 1].readChannel();
}