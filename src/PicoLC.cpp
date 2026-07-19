#include "PicoLCClass.h"
#include "internal/PicoLCPins.h"
#include "internal/Shifter.h"
#include "PicoLCInput.h"
#include "PicoLCTimer.h"

PicoLC::PicoLC()
    : Y0(0), Y1(1), Y2(2), Y3(3),
      X0(*this, 4), X1(*this, 5), X2(*this, 6), X3(*this, 7),
      P0(0), P1(2),
      StatusLED(6),
      RunStopButton(*this, 20),UserButton(*this, 21), BootButton(*this, 0) {
}

void PicoLC::begin() {
  pinMode(picolc::pins::CommTX, OUTPUT);
  pinMode(picolc::pins::CommRX, INPUT);
  pinMode(picolc::pins::CommTR, OUTPUT);
  digitalWrite(picolc::pins::CommTR, LOW);

  digitalWrite(picolc::pins::CommTR, LOW);
  pinMode(picolc::pins::OutputEnable, OUTPUT);
  digitalWrite(picolc::pins::OutputEnable, LOW);
  
  pinMode(picolc::pins::ScanLED, OUTPUT);
  digitalWrite(picolc::pins::ScanLED, LOW);

  pinMode(picolc::pins::OutputFault, INPUT);

  analogWriteFreq(1000);
  analogWriteRange(1000);

  PLCShifter::begin();
  BootButton.filterMillis(20);
}

void PicoLC::update() {
  _currentMillis = millis();
  _cycles++;
  X0.update(_currentMillis);
  X1.update(_currentMillis);
  X2.update(_currentMillis);
  X3.update(_currentMillis);
  Y0.update(_currentMillis);
  Y1.update(_currentMillis);
  Y2.update(_currentMillis);
  Y3.update(_currentMillis);
  StatusLED.update(_currentMillis);
  RunStopButton.update(_currentMillis);
  UserButton.update(_currentMillis);
  BootButton.update(_currentMillis);
}

uint32_t PicoLC::currentMillis() const {
  return _currentMillis;
}

uint32_t PicoLC::cycles() const {
  return _cycles;
}

void PicoLC::enableOutputs(){

}

void PicoLC::disableOutputs(){

}

bool PicoLC::outputsEnabled() const {
  return false;
}

bool PicoLC::outputFault() const {
  return false;
}

void PicoLC::resetOutputDriver() {

}

float PicoLC::supplyVoltage() const {
  return 0.0F;
}
