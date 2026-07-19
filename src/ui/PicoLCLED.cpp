#include <cmath>
#include <cstdint>
#include <algorithm>

#include "../PicoLC.h"

PicoLCLED::PicoLCLED(uint8_t channel) {
    _channel = channel;
    pinMode(_channel, OUTPUT);
    digitalWrite(_channel, LOW);
}

void PicoLCLED::patternMode(PatternMode mode) { _patternMode = mode; }
PatternMode PicoLCLED::patternMode() const { return _patternMode; }

void PicoLCLED::update(uint32_t currentMillis) {
    bool wireState = false;

    if( _patternMode == PatternMode::Pulse ) {
        if( _state ) {
            _pulseStart = currentMillis;
            _state = false;
        }
        if( _pulseStart > 0 && currentMillis < _pulseStart + _pulseLength ) {
            wireState = true;
        } else {
            _pulseStart = 0;
        }
    }
    if( _state ) {
        if( _patternMode == PatternMode::Blink ) {
            if( _blinkStart == 0 ) {
                _blinkStart = currentMillis;
            }
            _blinkStart += ((currentMillis - _blinkStart) / _blinkPeriod) * _blinkPeriod;
            if( currentMillis - _blinkStart < _blinkOnTime ) {
                wireState = true;
            }
        } else {
            wireState = true;
        }
    }
    if( !wireState ) {
        digitalWrite(_channel, LOW);
        return;
    }
    if( _brightness < PicoLCLED::PWM_MAX ) {
        analogWrite(_channel, wireState ? _brightness : 0);
    } else {
        digitalWrite(_channel, wireState ? HIGH : LOW);
    }
}

bool PicoLCLED::read() const {
    if( _patternMode == PatternMode::Pulse ) {
        return _pulseStart > 0;
    }
    return _state;
}

bool PicoLCLED::write(bool state) {
    if( _patternMode == PatternMode::Pulse && _pulseLength > 0 && !state ) {
        _pulseStart = 0;
    }
    _state = state;
    return _state;
}

void PicoLCLED::toggle() {
    if( _patternMode == PatternMode::Pulse && _pulseLength > 0 && !_state ) {
        write(false);
    } else {
        write(!_state);
    }
}

void PicoLCLED::pulseLength(uint32_t ms) { _pulseLength = ms; }
uint32_t PicoLCLED::pulseLength() const { return _pulseLength; }
void PicoLCLED::blinkPeriod(uint32_t ms) { _blinkPeriod = ms; }
uint32_t PicoLCLED::blinkPeriod() const { return _blinkPeriod; }
void PicoLCLED::blinkOnTime(uint32_t ms) { _blinkOnTime = ms; }
uint32_t PicoLCLED::blinkOnTime() const { return _blinkOnTime; }

void PicoLCLED::brightness(uint16_t brightness) {
    const uint32_t value = std::min<uint16_t>(brightness, 1000);

    _brightness = static_cast<uint16_t>((value * value * 1000UL + 5000UL) / 10000UL);
}
uint16_t PicoLCLED::brightness() const {
    uint16_t duty = std::min<uint16_t>(_brightness, 1000);
    return static_cast<uint16_t>(std::sqrt(duty / 1000.0F) * 1000.0F);
}
