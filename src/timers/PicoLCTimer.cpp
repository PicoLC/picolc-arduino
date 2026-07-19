#include <Arduino.h>
#include "PicoLCTimer.h"
#include "PicoLCClass.h"

PicoLCTimer::PicoLCTimer(PicoLC& plc, TimerMode mode, uint32_t preset) :
    _plc(plc),
    _mode(mode),
    _preset(preset),
    _started(0) {}

void PicoLCTimer::presetMillis(uint32_t ms) {
    _preset = ms;
}

uint32_t PicoLCTimer::presetMillis() const {
    return _preset;
}

bool PicoLCTimer::read() {
    uint32_t currentMillis = _plc.currentMillis();
    switch( _mode) {
        case TimerMode::TON:
            if( _running ) {
                return (currentMillis - _started) >= _preset;
            } else {
                return false;
            }
            break;
        case TimerMode::TOF:
            if( _running ) {
                return (currentMillis - _started) < _preset;
            } else {
                return true;
            }
            break;
        case TimerMode::TP:
            if( _running ) {
                if( currentMillis - _started >= _preset ) {
                    _running = false;
                    _started = 0;
                }
            }
            return _running;
            break;
    }
    return false;
}

void PicoLCTimer::write(bool input) {
    uint32_t currentMillis = _plc.currentMillis();
    switch( _mode) {
        case TimerMode::TON:
            if( input ) {
                if( !_running ) {
                    _started = currentMillis;
                    _running = true;
                }
            } else {
                _started = 0;
                _running = false;
            }
            break;
        case TimerMode::TOF:
            if( input ) {
                _started = 0;
                _running = false;
            } else {
                if( _started == 0 ) {
                    _started = currentMillis;
                    _running = true;
                }
            }
            break;
        case TimerMode::TP:
            if( _running ) {
                if( currentMillis - _started >= _preset ) {
                    _running = false;
                    _started = 0;
                }
            } else {
                if( input && !_lastInput) {
                    _running = true;
                    _started = currentMillis;
                }
            }
            break;
    }
    _lastInput = input;
}

bool PicoLCTimer::update(bool input) {
    write(input);
    return read();
}

bool PicoLCTimer::running() const {
    return _running;
}

uint32_t PicoLCTimer::elapsed() const {
    if( _started == 0 ) {
        return 0;
    }
    return _plc.currentMillis() - _started;
}

uint32_t PicoLCTimer::remaining() const {
    if( _started == 0 ) {
        return _preset;
    }
    uint32_t elapsed = _plc.currentMillis() - _started;
    if( elapsed >= _preset ) {
        return 0;
    }
    return _preset - elapsed;
}

void PicoLCTimer::reset() {
    _started = 0;
    _running = false;
}