/*
  Gate Access Integration

  Interconnects a 24v gate controller with a 12v Access Control System, adding warning lights and siren chirps.
  
  In this deployment, the PicoLC, light, and siren are powered by 12v, while the gate controller is 24v.  
  The isolated low-side inputs allow the systems to share a ground and communicate safely without 
  additional circuitry to translate the 24v signals to 12v levels.

  Relatively complex logic can be expressed in a few lines of code using the PicoLC library, making the solution
  easier to implement and maintain than a collection of relays with Arduino glue code.

*/

#include <PicoLC.h>

PicoLC plc;

// For more complex applications, you might want more readable aliases for the inputs and outputs.  
// This is optional, but can make the code easier to read.
auto& gateIsOpen   = plc.X0;
auto& gateIsMoving = plc.X1;
auto& acOpen       = plc.X2;
auto& radioOpen    = plc.X3;

auto& gateOpen = plc.Y0;
auto& acStatus = plc.Y1;
auto& light    = plc.Y2;
auto& chirp    = plc.Y3;

PicoLCTimer t_chirp(&plc, TimerMode::TP, 3000);
PicoLCTimer t_radioAllowed(&plc, TimerMode::TP, 5000);

void setup() {
  plc.begin();

  gateOpen.outputMode(OutputMode::LowSide);
  acStatus.outputMode(OutputMode::LowSide);
  light.outputMode(OutputMode::HighSide);
  chirp.outputMode(OutputMode::HighSide);

  light.blinkPeriod(1000);
  light.blinkOnTime(200);

  chirp.blinkPeriod(500);
  chirp.blinkOnTime(150);
}

void loop() {
  plc.update();

  // Light flashes any time the gate is open or moving.
  light.write( gateIsOpen.read() || gateIsMoving.read());

  // When gate starts moving, chirp for 3 seconds.
  chirp.write( t_chirp.update( gateIsMoving.read()));

  // Pass gate status through to access controller.
  acStatus.write( gateIsOpen.read());

  if( acOpen.read()) {
    // When access controller requests gate open, comply.  AC overrides radio.
    gateOpen.write( true );
  } else {
    // When wireless (radio) button pressed, open gate, but stop responding to radio if it stays active more than 5 seconds.
    gateOpen.write( t_radioAllowed.update( radioOpen.read()));
  }
}
