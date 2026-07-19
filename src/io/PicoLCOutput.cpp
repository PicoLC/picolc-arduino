#include "PicoLC.h"
#include "internal/Shifter.h"

PicoLCOutput::PicoLCOutput(uint8_t channel) : 
        _channel(channel), 
        _outputMode(OutputMode::HighSide),
        _patternMode(PatternMode::Steady),
        _pulseLength(0),
        _blinkPeriod(0),
        _blinkStart(0),
        _blinkTarget(UINT16_MAX),
        _blinkOnTime(UINT16_MAX),
        _duty(UINT16_MAX),
        _state(false) {
    duty(_duty);
}

void PicoLCOutput::update(uint32_t currentMillis) {
    uint8_t value = (PLCShifter::_lastOut) & ~(1 << _channel);

    if( _patternMode == PatternMode::Pulse ) {
        if( _state ) {
            _pulseStart = currentMillis;
            _state = false;
        }
        if( _pulseStart > 0 && currentMillis < _pulseStart + _pulseLength ) {
            value |= (1 << _channel);
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
                value |= (1 << _channel);
            }
        } else {
            value |= (1 << _channel);
        }
    }
    PLCShifter::out(value);
}

bool PicoLCOutput::read() const {
    if( _patternMode == PatternMode::Pulse ) {
        return _pulseStart > 0;
    }
    return _state;
}

bool PicoLCOutput::write(bool state) {
    if( _patternMode == PatternMode::Pulse && _pulseLength > 0 && !state ) {
        _pulseStart = 0;
    }
    _state = state;
    return _state;
}

void PicoLCOutput::pulseLength(uint32_t ms) { _pulseLength = ms; }
uint32_t PicoLCOutput::pulseLength() const { return _pulseLength; }

void PicoLCOutput::blinkPeriod(uint32_t ms) {
    _blinkPeriod = ms; 
    if( ms > 0 ) _blinkStart = millis();
}
uint32_t PicoLCOutput::blinkPeriod() const { return _blinkPeriod; }
void PicoLCOutput::blinkOnTime(uint32_t ms) {
    _blinkOnTime = ms;
}
uint32_t PicoLCOutput::blinkOnTime() const { return _blinkOnTime; }


void PicoLCOutput::outputMode(OutputMode mode) { _outputMode = mode; }
OutputMode PicoLCOutput::outputMode() const { return _outputMode; }

void PicoLCOutput::patternMode(PatternMode mode) { _patternMode = mode; }
PatternMode PicoLCOutput::patternMode() const { return _patternMode; }

uint32_t PicoLCOutput::frequency(uint32_t hertz) { _frequency = hertz; return _frequency; }
uint32_t PicoLCOutput::frequency() const { return _frequency; }
uint16_t PicoLCOutput::duty(uint16_t duty) { 
    _duty = std::max<uint16_t>(duty, PicoLCOutput::DUTY_MAX);
    return _duty;
}
uint16_t PicoLCOutput::duty() const { return _duty; }
