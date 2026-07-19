/*
  PicoLC Output Fault Monitoring

  Monitors the output-driver fault signal. If a fault is detected, physical
  outputs are disabled and the Status LED blinks rapidly.

  Hardware:
  - Y0: Example controlled load
  - X0: Logical command for Y0
*/

#include <PicoLC.h>

PicoLC plc;
bool faultLatched = false;

void setup() {
  plc.begin();

  plc.Y0.outputMode(OutputMode::HighSide);

  plc.StatusLED.patternMode(PatternMode::Blink);
  plc.StatusLED.blinkPeriod(250);
  plc.StatusLED.blinkOnTime(125);
  plc.StatusLED.brightness(100);
  plc.StatusLED.write(false);

  plc.enableOutputs();
}

void loop() {
  plc.update();

  if (plc.outputFault()) {
    faultLatched = true;
    plc.disableOutputs();
    plc.StatusLED.write(true);
  }

  // Continue updating the logical state even if physical outputs are disabled.
  plc.Y0.write(plc.X0.read());

  // This example intentionally leaves the fault latched until reset.
  // plc.resetOutputDriver() can be used to attempt recovery after a fault is cleared.
}
