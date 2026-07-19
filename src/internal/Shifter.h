#pragma once

#include "PicoLCPins.h"

class PLCShifter {

public:
    static uint8_t _lastOut;
    static uint8_t _lastIn;
    static void begin();
    static void out(uint8_t value);
    static uint8_t in();
};

