#include <cstdint>

struct Channel {
    int8_t pin;
    int8_t minLimit;
    int8_t maxLimit;
    int8_t defaultValue;
    int8_t defaultValueBool;

    Channel initialize(int8_t pin, int8_t minLimit, int8_t maxLimit, int8_t defaultValue);

    int8_t readChannel();
    
    bool readSwitch();
};