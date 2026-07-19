#include <Arduino.h>
#include "Shifter.h"

uint8_t PLCShifter::_lastOut = 0;
uint8_t PLCShifter::_lastIn = 0;

void PLCShifter::begin() {
    pinMode(picolc::pins::ShiftClock, OUTPUT);
    digitalWrite(picolc::pins::ShiftClock, LOW);
    pinMode(picolc::pins::ShiftOut, OUTPUT);
    pinMode(picolc::pins::ShiftIn, INPUT);
    pinMode(picolc::pins::ShiftOutLatch, OUTPUT);
    digitalWrite(picolc::pins::ShiftOutLatch, LOW);
    pinMode(picolc::pins::ShiftInLatch, OUTPUT);
    digitalWrite(picolc::pins::ShiftInLatch, LOW);
}

void PLCShifter::out(uint8_t value) {
    shiftOut(picolc::pins::ShiftOut, picolc::pins::ShiftClock, LSBFIRST, value);
    digitalWrite(picolc::pins::ShiftOutLatch, HIGH);
    digitalWrite(picolc::pins::ShiftOutLatch, LOW);
    PLCShifter::_lastOut = value;
}

uint8_t PLCShifter::in() {
    uint8_t value = 0;
    uint8_t i;
    digitalWrite(picolc::pins::ShiftInLatch, LOW);
    delayMicroseconds(20);
    digitalWrite(picolc::pins::ShiftInLatch, HIGH);
    delayMicroseconds(20);

    for( i = 0; i < 8; i++ ) {
        digitalWrite(picolc::pins::ShiftClock, LOW);
        delayMicroseconds(20);
        value |= (digitalRead(picolc::pins::ShiftIn) << (7-i));
        digitalWrite(picolc::pins::ShiftClock, HIGH);
        delayMicroseconds(20);
    }
    PLCShifter::_lastIn = value;
    return value;
}

