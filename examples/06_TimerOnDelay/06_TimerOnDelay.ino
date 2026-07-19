/*
  PicoLC TON Timer

  Turns Y0 on after X0 remains active for two seconds.

  Hardware:
  - X0: Timer input
  - Y0: Delayed output
*/

#include <PicoLC.h>

PicoLC plc;
// Timers must be created after the PicoLC instance and before begin() is called.
PicoLCTimer onDelay(plc, TimerMode::TON, 2000);

void setup() {
  plc.begin();

  plc.Y0.outputMode(OutputMode::HighSide);
  plc.enableOutputs();
}

void loop() {
  plc.update();

  // update(input) performs write(input), then read().
  plc.Y0.write(onDelay.update(plc.X0.read()));

  // This could also be expressed as two separate calls:
  // onDelay.write(plc.X0.read());
  // plc.Y0.write(onDelay.read());
}
