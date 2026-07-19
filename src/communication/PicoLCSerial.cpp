#include <Arduino.h>
#include "PicoLC.h"
#include "../internal/PicoLCPins.h"

PicoLCSerial::PicoLCSerial() {

}

void PicoLCSerial::begin(uint32_t baud, uint16_t config) {
    if( !_started ) {
        pinMode(picolc::pins::CommTR, OUTPUT);
        digitalWrite(picolc::pins::CommTR, LOW);
        Serial1.setTX(picolc::pins::CommTX);
        Serial1.setRX(picolc::pins::CommRX);
        Serial1.begin(baud, config);
        _started = true;
    }
}

void PicoLCSerial::end() {
    if( _started ) {
        Serial1.end();
        _started = false;
    }
}

int PicoLCSerial::available() { return _started ? Serial1.available() : 0; }
int PicoLCSerial::read() { return _started ? Serial1.read() : -1; }
int PicoLCSerial::peek() { return _started ? Serial1.peek() : -1; }

size_t PicoLCSerial::write(uint8_t value) { 
    if( _started ) {
        setTR(true);
        size_t result = Serial1.write(value);
        setTR(false);
        return result;
    }
    return 0;
}

size_t PicoLCSerial::write(const uint8_t* buffer, size_t size) {
    if( _started ) {
        setTR(true);
        size_t result = Serial1.write(buffer, size);
        setTR(false);
        return result;
    }
    return 0;
}

void PicoLCSerial::flush() {
    if( _started ) { Serial1.flush(); }
}

void PicoLCSerial::setTR(bool writing) {
    if( _started ) {
        if( writing ) {
            digitalWrite(picolc::pins::CommTR, HIGH);
            delay(1);
        } else {
            delay(1);
            digitalWrite(picolc::pins::CommTR, LOW);
        }
    }
}
bool PicoLCSerial::started() const { return _started; }
