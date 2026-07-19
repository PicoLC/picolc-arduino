#include "PicoLCClass.h"
#include "internal/Shifter.h"

PicoLCInput::PicoLCInput(PicoLC& plc, uint8_t channel) : _plc(plc), _channel(channel) {
}

void PicoLCInput::update(uint32_t currentMillis) {
    _lastValue = _currentValue;
    uint8_t value = PLCShifter::in();
    _currentValue = (value & (1 << _channel)) != 0;
    if( _inverted ) {
        _currentValue = !_currentValue;
    }
    if( _filterMillis == 0 ) {
        return;
    }
    if( _filterStart > 0 ) {
        if( _currentValue != _candidateValue) {
            _currentValue = _lastValue;
            _filterStart = currentMillis;
            return;
        }
        if( currentMillis - _filterStart >= _filterMillis ) {
            _filterStart = 0;
        } else {
            _currentValue = _lastValue;
        }
    } else {
        if( _currentValue != _lastValue ) {
            _filterStart = currentMillis;
            _candidateValue = _currentValue;
            _currentValue = _lastValue;
        }
    }
}

void PicoLCInput::inverted(bool invert) {
    _inverted = invert;
}

bool PicoLCInput::inverted() const {
    return _inverted;
}

void PicoLCInput::filterMillis(uint32_t ms) {
    _filterMillis = ms;
}

uint32_t PicoLCInput::filterMillis() const {
    return _filterMillis;
}

bool PicoLCInput::read() const { 
    return _currentValue;
}

bool PicoLCInput::rising() const {
    if( _plc.cycles() == 0 ) {
        return false;
    }
    return (_currentValue == true && _lastValue == false);
}

bool PicoLCInput::falling() const {
    if( _plc.cycles() == 0 ) {
        return false;
    }
    return (_currentValue == false && _lastValue == true);
}

bool PicoLCInput::changed() const {
    return (_currentValue != _lastValue);
}