#include "PicoLCClass.h"

PicoLCButton::PicoLCButton(PicoLC& plc, uint8_t channel) : _plc(plc), _channel(channel) {
}

void PicoLCButton::update(uint32_t currentMillis) {
    _lastValue = _currentValue;
    if( _channel == 0 ) {
        _currentValue = BOOTSEL;
    } else {
        _currentValue = digitalRead(_channel);
    }
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

void PicoLCButton::inverted(bool invert) {
    _inverted = invert;
}

void PicoLCButton::filterMillis(uint32_t ms) {
    _filterMillis = ms;
}

uint32_t PicoLCButton::filterMillis() const {
    return _filterMillis;
}

bool PicoLCButton::read() const { 
    return _currentValue;
}

bool PicoLCButton::rising() const {
    if( _plc.cycles() == 0 ) {
        return false;
    }
    return (_currentValue == true && _lastValue == false);
}

bool PicoLCButton::falling() const {
    if( _plc.cycles() == 0 ) {
        return false;
    }
    return (_currentValue == false && _lastValue == true);
}

bool PicoLCButton::changed() const {
    return (_currentValue != _lastValue);
}