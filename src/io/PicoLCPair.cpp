#include "PicoLC.h"

PicoLCPair::PicoLCPair(uint8_t firstChannel)
    : _channel0(firstChannel), _channel1(firstChannel + 1),
      _requestedMode(PairMode::Coast) {}

void PicoLCPair::pair() {}
void PicoLCPair::unpair() {}
bool PicoLCPair::paired() const { return false; }
PairMode PicoLCPair::read() const { return _requestedMode; }
bool PicoLCPair::write(PairMode mode) { _requestedMode = mode; return false; }
bool PicoLCPair::write(PairMode mode, uint16_t) { return write(mode); }
bool PicoLCPair::writePercent(PairMode mode, float) { return write(mode); }
uint32_t PicoLCPair::frequency(uint32_t) { return 0; }
uint32_t PicoLCPair::frequency() const { return 0; }
uint16_t PicoLCPair::duty(uint16_t) { return 0; }
float PicoLCPair::dutyPercent(float) { return 0.0F; }
uint16_t PicoLCPair::duty() const { return 0; }
float PicoLCPair::dutyPercent() const { return 0.0F; }
void PicoLCPair::transitionDelayMillis(uint32_t) {}
uint32_t PicoLCPair::transitionDelayMillis() const { return 0; }
