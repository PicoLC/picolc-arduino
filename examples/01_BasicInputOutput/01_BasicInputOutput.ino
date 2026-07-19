/*
  PicoLC Basic Input and Output

  Mirrors logical input X0 to output Y0.

  Hardware:
  - X0: Input signal
  - Y0: Load connected for high-side operation
*/

#include <PicoLC.h>

PicoLC plc;

void setup() {
  plc.begin();

  // Select the electrical drive mode for Y0.
  plc.Y0.outputMode(OutputMode::HighSide);

  // Physical outputs remain disabled until explicitly enabled.
  plc.enableOutputs();
}

void loop() {
  // Start a new PicoLC scan.
  plc.update();

  // PicoLC works with logical states.
  plc.Y0.write(plc.X0.read());
}
