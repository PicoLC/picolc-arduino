/*
  PicoLC Input Filtering

  Mirrors X0 to Y0 after the input remains stable for 20 milliseconds.

  Hardware:
  - X0: Mechanical switch, relay contact, or other binary input
  - Y0: Load connected for high-side operation
*/

#include <PicoLC.h>

PicoLC plc;

void setup() {
  plc.begin();

  // Require X0 to remain stable for 20 ms before its logical state changes.
  plc.X0.filterMillis(20);

  plc.Y0.outputMode(OutputMode::HighSide);
  plc.enableOutputs();
}

void loop() {
  plc.update();

  // read() returns the filtered logical state.
  plc.Y0.write(plc.X0.read());
}
