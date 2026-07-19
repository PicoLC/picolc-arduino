/*
  PicoLC LED Patterns

  Cycles the Status LED through steady, pulse, and blink modes each time the
  BOOTSEL button is pressed.

  The configured pulse and blink timing remains stored while modes change.
*/

#include <PicoLC.h>

PicoLC plc;
uint8_t modeIndex = 0;

void setup() {
  plc.begin();

  plc.BootButton.filterMillis(20);

  // All timing values are configured in milliseconds.
  plc.StatusLED.pulseLength(500);
  plc.StatusLED.blinkPeriod(1000);
  plc.StatusLED.blinkOnTime(150);

  // Brightness is configured as a percentage of the maximum value (0-100).
  plc.StatusLED.brightness(50);

  plc.StatusLED.patternMode(PatternMode::Steady);
  plc.StatusLED.write(true);
}

void loop() {
  plc.update();

  if (plc.BootButton.rising()) {
    modeIndex = (modeIndex + 1) % 3;

    switch (modeIndex) {
      case 0:
        plc.StatusLED.patternMode(PatternMode::Steady);
        plc.StatusLED.write(true);
        break;

      case 1:
        plc.StatusLED.patternMode(PatternMode::Pulse);
        plc.StatusLED.write(true);
        break;

      case 2:
        plc.StatusLED.patternMode(PatternMode::Blink);
        plc.StatusLED.write(true);
        break;
    }
  }
}
