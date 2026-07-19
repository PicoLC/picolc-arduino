/*
  PicoLC TOF Timer

  Turns Y0 on immediately with X0, then keeps Y0 on for two seconds after
  X0 becomes inactive.

  Hardware:
  - X0: Timer input
  - Y0: Delayed-off output
*/

#include <PicoLC.h>

PicoLC plc;
PicoLCTimer offDelay(plc, TimerMode::TOF, 2000);

void setup() {
  plc.begin();

  plc.Y0.outputMode(OutputMode::HighSide);
  plc.enableOutputs();
}

void loop() {
  plc.update();

  plc.Y0.write(offDelay.update(plc.X0.read()));
}
