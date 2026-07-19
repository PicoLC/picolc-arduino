#pragma once

#include <Arduino.h>

namespace picolc::pins {
    constexpr uint8_t CommTX = 0;
    constexpr uint8_t CommRX = 1;

    constexpr uint8_t CommTR = 2;
    constexpr uint8_t OutputEnable = 3;
    constexpr uint8_t ShiftOutLatch = 4;
    constexpr uint8_t ShiftInLatch = 5;
    constexpr uint8_t ScanLED = 6;

    constexpr uint8_t ShiftClock = 10;
    constexpr uint8_t ShiftOut = 11;
    constexpr uint8_t ShiftIn = 12;
    constexpr uint8_t OutputFault = 13;

    constexpr uint8_t SDA = 14;
    constexpr uint8_t SCL = 15;
    
    //constexpr uint8_t CommDE = 2;
    //constexpr uint8_t CommRE = 3;

    //constexpr uint8_t BoardReset = 4;
    //constexpr uint8_t OutputEnable = 5;

    //constexpr uint8_t Y0 = 6;
    //constexpr uint8_t Y1 = 7;
    //constexpr uint8_t Y2 = 8;
    //constexpr uint8_t Y3 = 9;

    //constexpr uint8_t Y0Dir = 10;
    //constexpr uint8_t Y1Dir = 11;
    //constexpr uint8_t Y2Dir = 12;
    //constexpr uint8_t Y3Dir = 13;

    //constexpr uint8_t X0 = 14;
    //constexpr uint8_t X1 = 15;
    //constexpr uint8_t X2 = 16;
    //constexpr uint8_t X3 = 17;

    //constexpr uint8_t UserButton = 18;
    //constexpr uint8_t RunStopButton = 19;

    //constexpr uint8_t SDA = 20;
    //constexpr uint8_t SCL = 21;

    //constexpr uint8_t OutputFault = 22;

    //constexpr uint8_t StatusLED = 26;
    //constexpr uint8_t FaultLED = 27;

    //constexpr uint8_t VinSense = 28;
}