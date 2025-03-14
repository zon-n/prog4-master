#include <Arduino.h>
#include "channel.h"

struct Input
{
    static constexpr int8_t MIN_CANAL[6] = {-100, -100, 0, -100, 0, 0};
    static constexpr int8_t MAX_CANAL[6] = {100, 100, 100, 100, 100, 100};
    static constexpr int8_t DEFAULT_CANAL[6] = {0, 0, 0, 0, 0, 0};
    Channel channels[6] = {Channel(), Channel(), Channel(), Channel(), Channel(), Channel()};

    void start(int8_t ch1Pin, int8_t ch2Pin, int8_t ch3Pin, int8_t ch4Pin, int8_t ch5Pin, int8_t ch6Pin);

    int8_t getChannel(u_int8_t channel);
};
