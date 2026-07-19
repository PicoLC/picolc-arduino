/*
  PicoLC Global Output Enable

  X0 controls the logical state of Y0. The BOOTSEL button toggles whether
  physical output drive is globally enabled.

  The Y0 indicator continues to show what would be output even while physical
  outputs are disabled.

  Hardware:
  - X0: Logical output command
  - Y0: Controlled load
  - BOOTSEL: Toggle physical-output enable
*/

#include <PicoLC.h>

PicoLC plc;
bool outputsEnabled = false;

void setup() {
  plc.begin();

  plc.X0.filterMillis(20);
  plc.BootButton.filterMillis(20);

  plc.Y0.outputMode(OutputMode::HighSide);

  // Leave physical outputs disabled at startup.
  plc.disableOutputs();
}

void loop() {
  plc.update();

  plc.Y0.write(plc.X0.read());

  if (plc.BootButton.rising()) {
    outputsEnabled = !outputsEnabled;

    if (outputsEnabled) {
      plc.enableOutputs();
    } else {
      plc.disableOutputs();
    }
  }
}
