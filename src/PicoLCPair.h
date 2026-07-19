#pragma once

#include <Arduino.h>
#include "PicoLCTypes.h"

class PicoLCPair {
    friend class PicoLC;

private:
    explicit PicoLCPair(uint8_t firstChannel);
    uint8_t _channel0, _channel1;
    PairMode _requestedMode;

public:
    void pair();
    void unpair();
    bool paired() const;

    PairMode read() const;
    bool write(PairMode mode);
    bool write(PairMode mode, uint16_t duty);
    bool writePercent(PairMode mode, float percent);

    uint32_t frequency(uint32_t hertz);
    uint32_t frequency() const;

    uint16_t duty(uint16_t duty);
    float dutyPercent(float pct);
    uint16_t duty() const;
    float dutyPercent() const;

    void transitionDelayMillis(uint32_t ms);
    uint32_t transitionDelayMillis() const;
};

