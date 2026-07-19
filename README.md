# PicoLC Arduino Library

PicoLC is an Arduino library for the PicoLC controller, a compact RP2040/RP2350-based control board designed for 12 V and 24 V automation projects.

The library provides a logical, scan-based interface for:

- Four protected field inputs
- Four configurable field outputs
- Output pairing
- Status indicators and board buttons
- IEC-style timers
- RS-485 communication
- Nonvolatile user storage
- Board-level diagnostics

PicoLC uses the [Earle Philhower Arduino-Pico core](https://github.com/earlephilhower/arduino-pico) and is not intended to support other Arduino cores.

## Status

This library is under active development.

The public API may change while the hardware and library are being validated. Storage and paired-output support are still being finalized.

## Requirements

- PicoLC hardware
- Raspberry Pi Pico, Pico W, Pico 2, or Pico 2 W as supported by the PicoLC board
- Arduino IDE or Arduino CLI
- Earle Philhower Arduino-Pico core

## Installation

Install the Earle Philhower Arduino-Pico core, then install this library into your Arduino libraries directory.

For local development, place or link the repository at:

```text
Documents/Arduino/libraries/PicoLC
```

Restart the Arduino IDE after adding or updating the library.

## Basic Usage

```cpp
#include <PicoLC.h>

PicoLC plc;

void setup() {
    plc.begin();

    plc.Y0.outputMode(OutputMode::HighSide);

    // Physical outputs remain disabled until explicitly enabled.
    plc.enableOutputs();
}

void loop() {
    // Begin a new PicoLC scan.
    plc.update();

    // Mirror logical input X0 to logical output Y0.
    plc.Y0.write(plc.X0.read());
}
```

## Scan Model

Call `plc.update()` once at the beginning of each `loop()` iteration.

`update()`:

- Captures one shared millisecond timestamp for the scan
- Increments the scan counter
- Samples and filters inputs and buttons
- Updates edge-detection state
- Advances output and LED pulse, blink, and PWM behavior
- Applies configured logical output behavior to the board

Timed API components use the timestamp captured by the most recent `plc.update()`. This keeps timers, filters, pulses, and blink patterns consistent within one application scan.

## Logical and Physical States

The public API works with logical states.

```cpp
bool inputActive = plc.X0.read();
plc.Y0.write(inputActive);
```

Electrical output behavior is configured separately:

```cpp
plc.Y0.outputMode(OutputMode::HighSide);
plc.Y1.outputMode(OutputMode::LowSide);
plc.Y2.outputMode(OutputMode::PushPull);
```

Physical output drive is globally controlled through:

```cpp
plc.enableOutputs();
plc.disableOutputs();
```

Disabling physical outputs preserves logical output states. Output indicator LEDs continue to display the states that would otherwise be sent to the physical outputs.

## Inputs

PicoLC inputs expose filtered logical state and edge detection:

```cpp
plc.X0.filterMillis(20);

bool active = plc.X0.read();

if (plc.X0.rising()) {
    // Logical false-to-true transition
}

if (plc.X0.falling()) {
    // Logical true-to-false transition
}

if (plc.X0.changed()) {
    // Any logical transition
}
```

Filtering and inversion are applied before logical state changes and edge detection.

Edge results remain valid until the next `plc.update()`.

## Outputs

Each output has an electrical mode, a pattern mode, and a linear PWM duty value.

```cpp
plc.Y0.outputMode(OutputMode::HighSide);
plc.Y0.patternMode(PatternMode::Steady);
plc.Y0.duty(1000);
plc.Y0.write(true);
```

The output duty range is:

```text
0     = 0%
500   = 50%
1000  = 100%
```

### Output Patterns

```cpp
plc.Y0.patternMode(PatternMode::Steady);
```

Applies the logical state continuously.

```cpp
plc.Y1.patternMode(PatternMode::Pulse);
plc.Y1.pulseLength(1000);
plc.Y1.write(true);
```

Starts or restarts a fixed-duration pulse. Writing `false` stops the pulse at the next update() cycle.

```cpp
plc.Y2.patternMode(PatternMode::Blink);
plc.Y2.blinkPeriod(1000);
plc.Y2.blinkOnTime(200);
plc.Y2.write(true);
```

Starts a repeating blink pattern in the on phase. Writing `false` stops blinking at the next update() cycle.

## LEDs

Board LEDs use the same steady, pulse, and blink pattern modes as outputs.

```cpp
plc.StatusLED.patternMode(PatternMode::Blink);
plc.StatusLED.blinkPeriod(1000);
plc.StatusLED.blinkOnTime(100);
plc.StatusLED.brightness(50);
plc.StatusLED.write(true);
```

LED brightness uses a user-facing range of `0` through `100`. The library converts this to a nonlinear PWM value to provide more natural perceived brightness changes.

A brightness of zero does not change the logical LED state or configured pattern.

## Buttons

Board buttons expose the same filtered logical-state and edge-detection interface as field inputs:

```cpp
plc.BootButton.filterMillis(20);

if (plc.BootButton.rising()) {
    plc.StatusLED.toggle();
}
```

`read()` returns `true` when the button is logically pressed.

## Timers

PicoLC timers provide standard IEC-style `TON`, `TOF`, and `TP` behavior using the shared PicoLC scan timebase.

```cpp
PicoLCTimer onDelay(plc, TimerMode::TON, 2000);
```

The compact form performs `write(input)` followed by `read()`:

```cpp
plc.Y0.write(onDelay.update(plc.X0.read()));
```

The operations may also be called separately:

```cpp
onDelay.write(plc.X0.read());
bool output = onDelay.read();
```

Additional timer state is available through:

```cpp
onDelay.running();
onDelay.elapsed();
onDelay.remaining();
onDelay.presetMillis();
onDelay.reset();
```

Timers should normally be serviced once per PicoLC scan.

## RS-485 Serial

`plc.Serial` provides an Arduino `Stream`-compatible interface to the PicoLC RS-485 port.

```cpp
void setup() {
    plc.begin();
    plc.Serial.begin(19200);
}

void loop() {
    plc.update();

    if (plc.Serial.available()) {
        int value = plc.Serial.read();
        plc.Serial.write(static_cast<uint8_t>(value));
    }
}
```

The current implementation is blocking. A write call returns after transmission is complete and the RS-485 transceiver has returned to receive mode.

`plc.Serial` is separate from the Arduino USB `Serial` interface.

## Board Diagnostics

### Output enable state

```cpp
bool enabled = plc.outputsEnabled();
```

### Output-driver fault

```cpp
if (plc.outputFault()) {
    plc.disableOutputs();
}
```

### Output-driver reset

```cpp
plc.resetOutputDriver();
```

### Supply voltage

```cpp
float volts = plc.supplyVoltage();
```

`supplyVoltage()` samples the board VIN sense circuit when called and is independent of `plc.update()`.

## Examples

The library includes progressive examples covering:

1. Basic input and output
2. Input filtering
3. Input edge detection
4. Output patterns
5. LED patterns
6. TON timer
7. TOF timer
8. TP timer
9. Global output enable
10. Output fault monitoring

Open them through the Arduino IDE under:

```text
File > Examples > PicoLC
```

## Repository Layout

```text
PicoLC/
├── examples/
├── src/
│   ├── PicoLC.h
│   ├── PicoLCClass.h
│   ├── PicoLCInput.h
│   ├── PicoLCOutput.h
│   ├── PicoLCLED.h
│   ├── PicoLCButton.h
│   ├── PicoLCTimer.h
│   ├── PicoLCSerial.h
│   └── ...
├── library.properties
├── LICENSE
└── README.md
```

## Hardware Notes

PicoLC is designed around fixed board resources and the Earle Philhower RP2040/RP2350 Arduino core.

Some hardware resources are shared:

- Outputs Y0 and Y1 are on the same RP2 PWM slice and share a PWM frequency
- Outputs Y2 and Y3 also share a PWM frequency.
- Individual outputs retain independent PWM duty values
- Output pairs may impose additional shared configuration requirements
- Physical outputs remain disabled until explicitly enabled

## License

Released under MIT license.  See LICENSE file.

## Project

Project website: [picolc.com](https://picolc.com)