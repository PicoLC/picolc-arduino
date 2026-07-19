#pragma once

#include <Arduino.h>

#include "PicoLCTypes.h"

class PicoLC;

/**
 * @brief IEC-style timer using the shared PicoLC scan timebase.
 *
 * PicoLCTimer supports standard TON, TOF, and TP behavior. Timer state is
 * evaluated using the timestamp captured at the start of the most recent
 * PicoLC::update().
 *
 * A timer should normally be serviced once per PicoLC scan.
 */
class PicoLCTimer {
private:
    PicoLC& _plc;
    TimerMode _mode;
    uint32_t _preset = 0;
    uint32_t _started = 0;
    bool _running = false;
    bool _lastInput = false;

public:
    /**
     * @brief Constructs a timer.
     *
     * @param plc PicoLC instance that provides the shared scan timebase.
     * @param mode Standard timer operating mode.
     * @param preset Initial timer preset in milliseconds.
     */
    PicoLCTimer(PicoLC& plc, TimerMode mode, uint32_t preset);

    /**
     * @brief Sets the timer preset.
     *
     * Changing the preset does not alter the accumulated timer value. The new
     * preset is used by the next read(), write(), or update() call and may
     * immediately change the timer output or running state.
     *
     * @param ms Timer preset in milliseconds.
     */
    void presetMillis(uint32_t ms);

    /**
     * @brief Returns the configured timer preset.
     *
     * @return Timer preset in milliseconds.
     */
    uint32_t presetMillis() const;

    /**
     * @brief Applies a timer input and returns the resulting output state.
     *
     * This is equivalent to calling write(input) followed by read().
     *
     * @param input Current logical timer input.
     * @return Timer Q state.
     */
    bool update(bool input);

    /**
     * @brief Returns the timer output state.
     *
     * The returned value represents the timer Q state evaluated against the
     * timestamp captured at the start of the most recent PicoLC::update().
     * The call may complete a time-dependent state transition that does not
     * require a new input value.
     *
     * @return Current timer Q state.
     */
    bool read();

    /**
     * @brief Applies the current logical input to the timer.
     *
     * The input is processed according to the configured standard TON, TOF,
     * or TP behavior. This method should normally be called once per PicoLC
     * scan.
     *
     * @param input Current logical timer input.
     */
    void write(bool input);

    /**
     * @brief Reports whether the timer is currently accumulating time.
     *
     * This is independent of the current input and output states.
     *
     * @return true while the timer is accumulating elapsed time.
     */
    bool running() const;

    /**
     * @brief Returns the accumulated timer value.
     *
     * @return Elapsed timer value in milliseconds.
     */
    uint32_t elapsed() const;

    /**
     * @brief Returns the time remaining before the preset is reached.
     *
     * @return Remaining timer value in milliseconds.
     */
    uint32_t remaining() const;

    /**
     * @brief Stops and clears the current timer operation.
     *
     * After reset(), a subsequent read() returns the same output state as a
     * timer of the configured mode that has never been started.
     */
    void reset();
};