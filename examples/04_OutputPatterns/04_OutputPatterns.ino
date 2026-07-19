/*
  PicoLC Output Patterns

  Demonstrates steady, pulse, and blink behavior on three outputs.

  Hardware:
  - X0: Steady-output enable
  - X1: Pulse trigger
  - X2: Blink enable
  - Y0: Steady output
  - Y1: One-second pulse output
  - Y2: Repeating blink output
*/

#include <PicoLC.h>

PicoLC plc;

void setup() {
  plc.begin();

  plc.X1.filterMillis(20);

  plc.Y0.outputMode(OutputMode::HighSide);
  plc.Y1.outputMode(OutputMode::HighSide);
  plc.Y2.outputMode(OutputMode::HighSide);

  plc.Y0.patternMode(PatternMode::Steady);

  plc.Y1.patternMode(PatternMode::Pulse);
  // Pulse length is configured in milliseconds. 1000 ms = 1 second.
  plc.Y1.pulseLength(1000);

  plc.Y2.patternMode(PatternMode::Blink);
  // Blink period and on time are also configured in milliseconds.
  plc.Y2.blinkPeriod(1000);
  plc.Y2.blinkOnTime(200);

  plc.enableOutputs();
}

void loop() {
  plc.update();

  // Steady mode follows the logical input state.
  plc.Y0.write(plc.X0.read());

  // Trigger one pulse when X1 becomes logically active.
  if (plc.X1.rising()) {
    plc.Y1.write(true);
  }

  // Blink while X2 is logically active.
  plc.Y2.write(plc.X2.read());
}
