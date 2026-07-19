#pragma once

#include <Arduino.h>

#include "PicoLCTypes.h"

#include "PicoLCInput.h"
#include "PicoLCOutput.h"
#include "PicoLCLED.h"
#include "PicoLCButton.h"
#include "PicoLCPair.h"
#include "PicoLCSerial.h"
#include "PicoLCStorage.h"

/**
 * @brief Main interface to PicoLC hardware and shared runtime services.
 *
 * PicoLC owns the board inputs, outputs, output pairs, indicators, buttons,
 * serial interface, and storage objects. It also provides the shared scan
 * timestamp used throughout the API.
 *
 * Call begin() once during setup(), configure the required hardware objects,
 * then call update() once at the beginning of each loop() iteration.
 */
class PicoLC {
private:
    uint32_t _currentMillis = 0;
    uint32_t _cycles = 0;

    PicoLC(const PicoLC&) = delete;
    PicoLC& operator=(const PicoLC&) = delete;

public:
    /**
     * @brief Constructs the PicoLC interface and its child hardware objects.
     *
     * Hardware initialization is deferred until begin() is called.
     */
    PicoLC();

    /** Hardware outputs Y0 through Y3. */
    PicoLCOutput Y0, Y1, Y2, Y3;

    /** Hardware inputs X0 through X3. */
    PicoLCInput X0, X1, X2, X3;

    /** Paired-output controllers for Y0/Y1 and Y2/Y3. */
    PicoLCPair P0, P1;

    /** Board status indicator LED. */
    PicoLCLED StatusLED;

    /** Board-defined control buttons. */
    PicoLCButton RunStopButton, UserButton, BootButton;

    /** RS-485 serial interface. */
    PicoLCSerial Serial;

    /** User-accessible nonvolatile storage. */
    PicoLCStorage Storage;

    /**
     * @brief Initializes PicoLC hardware and child objects.
     *
     * Call this once from setup() before configuring or using PicoLC inputs,
     * outputs, LEDs, buttons, serial, storage, or output pairs.
     *
     * Physical outputs remain disabled until enableOutputs() is called.
     */
    void begin();

    /**
     * @brief Advances PicoLC by one application scan.
     *
     * update() captures the shared millisecond timestamp, increments the scan
     * counter, samples and filters logical inputs and buttons, and advances
     * timed LED and output behavior.
     *
     * Call this once at the beginning of each loop() iteration. Logical output
     * commands issued after the call are applied during a subsequent update().
     */
    void update();

    /**
     * @brief Returns the timestamp captured for the current scan.
     *
     * The value changes only when update() is called and is shared by timed
     * API components such as timers, input filters, pulses, and blink patterns.
     *
     * @return Current scan timestamp in milliseconds.
     */
    uint32_t currentMillis() const;

    /**
     * @brief Returns the number of completed update cycles.
     *
     * Cycle zero represents initialization. The first call to update()
     * advances the counter to one.
     *
     * @return Current PicoLC scan count.
     */
    uint32_t cycles() const;

    /**
     * @brief Enables physical output-driver operation.
     *
     * Logical output states and indicator LEDs continue to operate regardless
     * of this setting. Enabling outputs permits the configured logical output
     * states to reach the physical output terminals.
     */
    void enableOutputs();

    /**
     * @brief Disables all physical output-driver channels.
     *
     * Logical output states are preserved, and the output indicator LEDs
     * continue to show the states that would otherwise be applied to the
     * hardware outputs.
     */
    void disableOutputs();

    /**
     * @brief Reports whether physical outputs are globally enabled.
     *
     * @return true when logical output states are permitted to drive the
     *         physical output terminals.
     */
    bool outputsEnabled() const;

    /**
     * @brief Reports the current output-driver fault indication.
     *
     * @return true when the hardware output driver reports a fault.
     */
    bool outputFault() const;

    /**
     * @brief Resets the hardware output driver.
     *
     * This operation resets the driver hardware without changing the logical
     * output configuration or commanded states.
     */
    void resetOutputDriver();

    /**
     * @brief Measures the PicoLC supply voltage.
     *
     * The value is sampled directly from the board VIN sense circuit when this
     * method is called and is independent of update().
     *
     * @return Measured supply voltage in volts.
     */
    float supplyVoltage() const;
};