/*
  PicoLC TP Timer

  Generates one three-second output pulse when X0 has a rising edge.

  Hardware:
  - X0: Pulse trigger
  - Y0: Pulsed output
*/

#include <PicoLC.h>

PicoLC plc;
PicoLCTimer pulseTimer(plc, TimerMode::TP, 3000);

void setup() {
  plc.begin();

  plc.X0.filterMillis(20);

  plc.Y0.outputMode(OutputMode::HighSide);
  plc.enableOutputs();
}

void loop() {
  plc.update();

  // Standard TP behavior starts a fixed pulse on a rising input edge.
  plc.Y0.write(pulseTimer.update(plc.X0.read()));

  // Additional timer functions are also available:
  // .running() returns true while the timer is active.
  // .elapsed() returns the elapsed time in milliseconds since the timer started.
  // .remaining() returns the remaining time in milliseconds for an active timer.
  // .reset() stops an active timer and resets the timer.
}
