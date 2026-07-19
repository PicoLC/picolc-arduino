#pragma once

#include <Arduino.h>

#include "PicoLCTypes.h"

/**
 * @brief Represents one configurable PicoLC hardware output.
 *
 * PicoLCOutput exposes logical output behavior rather than instantaneous
 * electrical state. The selected OutputMode defines the electrical drive
 * configuration, PatternMode defines timed behavior, and duty() defines the
 * linear PWM duty applied while the output pattern is active.
 *
 * Instances are created and owned by PicoLC and are accessed through members
 * such as PicoLC::Y0 through PicoLC::Y3.
 */
class PicoLCOutput {
    friend class PicoLC;

public:
    /** Maximum user-facing PWM duty value. */
    static constexpr uint16_t DUTY_MAX = 1000;

    /** Default PWM frequency in hertz. */
    static constexpr uint32_t FREQUENCY_DEFAULT = 1000;

private:
    /**
     * @brief Constructs an output associated with a hardware channel.
     *
     * PicoLC creates and initializes output instances during begin().
     *
     * @param channel Zero-based PicoLC output channel.
     */
    explicit PicoLCOutput(uint8_t channel);

    PicoLCOutput(const PicoLCOutput&) = delete;
    PicoLCOutput& operator=(const PicoLCOutput&) = delete;

    uint8_t _channel;
    uint32_t _pulseStart = 0;
    uint32_t _pulseLength = 0;
    uint32_t _frequency = FREQUENCY_DEFAULT;
    uint32_t _blinkPeriod = 0;
    uint32_t _blinkStart = 0;
    uint32_t _blinkTarget = 0;
    uint32_t _blinkOnTime = 0;
    uint16_t _duty = DUTY_MAX;
    bool _state = false;
    OutputMode _outputMode = OutputMode::LowSide;
    PatternMode _patternMode = PatternMode::Steady;

    /**
     * @brief Advances pulse, blink, and PWM output behavior for one scan.
     *
     * This method is called internally by PicoLC::update() using the shared
     * PicoLC scan timestamp.
     *
     * @param currentMillis Current PicoLC scan time in milliseconds.
     */
    void update(uint32_t currentMillis);

public:
    /**
     * @brief Returns the current logical output state.
     *
     * The returned value never represents the instantaneous electrical output
     * state. In Pulse mode, read() returns false after a pulse is triggered,
     * even while the timed pulse remains active.
     *
     * @return Current logical output state.
     */
    bool read() const;

    /**
     * @brief Applies a logical command to the output.
     *
     * In Steady and Blink modes, the supplied value becomes the logical output
     * state. In Blink mode, a false-to-true transition starts a new cycle in
     * the on phase, and false stops the pattern immediately.
     *
     * In Pulse mode, true starts or restarts the pulse timer and then returns
     * the logical state to false. Writing false stops an active pulse
     * immediately.
     *
     * Physical output drive remains disabled until PicoLC::enableOutputs() is
     * called.
     *
     * @param state Logical command to apply.
     * @return The supplied logical command, allowing the call to be chained
     *         in an expression.
     */
    bool write(bool state);

    /**
     * @brief Sets the electrical drive mode.
     *
     * @param mode Desired electrical output configuration.
     */
    void outputMode(OutputMode mode);

    /**
     * @brief Returns the configured electrical drive mode.
     *
     * @return Current OutputMode.
     */
    OutputMode outputMode() const;

    /**
     * @brief Sets the timed behavior applied to the output.
     *
     * Pulse and blink configuration values are retained when changing modes.
     *
     * @param mode Desired PatternMode.
     */
    void patternMode(PatternMode mode);

    /**
     * @brief Returns the configured timed output behavior.
     *
     * @return Current PatternMode.
     */
    PatternMode patternMode() const;

    /**
     * @brief Sets the Pulse-mode duration.
     *
     * @param ms Pulse duration in milliseconds.
     */
    void pulseLength(uint32_t ms);

    /**
     * @brief Returns the configured Pulse-mode duration.
     *
     * @return Pulse duration in milliseconds.
     */
    uint32_t pulseLength() const;

    /**
     * @brief Sets the complete Blink-mode cycle period.
     *
     * Changing the period does not restart the current cycle. The new timing
     * is applied during the next PicoLC::update().
     *
     * @param ms Blink period in milliseconds.
     */
    void blinkPeriod(uint32_t ms);

    /**
     * @brief Returns the complete Blink-mode cycle period.
     *
     * @return Blink period in milliseconds.
     */
    uint32_t blinkPeriod() const;

    /**
     * @brief Sets the active time within each Blink-mode cycle.
     *
     * @param ms Blink on-time in milliseconds.
     */
    void blinkOnTime(uint32_t ms);

    /**
     * @brief Returns the active time within each Blink-mode cycle.
     *
     * @return Blink on-time in milliseconds.
     */
    uint32_t blinkOnTime() const;

    /**
     * @brief Sets the requested PWM frequency.
     *
     * Outputs sharing an RP2 PWM slice also share a frequency. The returned
     * value is the frequency actually applied after hardware constraints and
     * shared-slice behavior are taken into account.
     *
     * @param hertz Requested frequency in hertz.
     * @return Actual configured frequency in hertz.
     */
    uint32_t frequency(uint32_t hertz);

    /**
     * @brief Returns the configured PWM frequency.
     *
     * @return PWM frequency in hertz.
     */
    uint32_t frequency() const;

    /**
     * @brief Sets the linear PWM duty.
     *
     * The public duty range is 0 through DUTY_MAX. A value of 0 produces no
     * active drive during an enabled pattern, and DUTY_MAX produces full duty.
     *
     * @param duty Requested duty value.
     * @return Applied duty value after range handling.
     */
    uint16_t duty(uint16_t duty);

    /**
     * @brief Returns the configured linear PWM duty.
     *
     * @return Duty value from 0 through DUTY_MAX.
     */
    uint16_t duty() const;
};