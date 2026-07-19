/*
  PicoLC Input Edge Detection

  Uses the BOOTSEL button to increment a four-bit binary counter displayed
  on outputs Y0 through Y3.

  Hardware:
  - BOOTSEL: Counter button
  - Y0-Y3: Four indicator loads or test LEDs

  Y3 is the least-significant bit and Y0 is the most-significant bit.
*/

#include <PicoLC.h>

PicoLC plc;
uint8_t counter = 0;

void setup() {
  plc.begin();

  plc.BootButton.filterMillis(20);

  plc.Y0.outputMode(OutputMode::HighSide);
  plc.Y1.outputMode(OutputMode::HighSide);
  plc.Y2.outputMode(OutputMode::HighSide);
  plc.Y3.outputMode(OutputMode::HighSide);

  plc.enableOutputs();
}

void loop() {
  plc.update();

  // rising() remains true for the current scan only.
  if (plc.BootButton.rising()) {
    counter = (counter + 1) % 16;

    plc.Y3.write((counter & 0x01) != 0);
    plc.Y2.write((counter & 0x02) != 0);
    plc.Y1.write((counter & 0x04) != 0);
    plc.Y0.write((counter & 0x08) != 0);
  }
}
