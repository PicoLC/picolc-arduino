#pragma once

#include <Arduino.h>

#include "PicoLCTypes.h"

/**
 * @brief Represents one configurable PicoLC status LED.
 *
 * PicoLCLED exposes logical LED behavior rather than instantaneous physical
 * output state. PatternMode controls steady, pulse, and blink behavior, while
 * brightness() controls perceived LED brightness using a user-facing
 * percentage scale.
 *
 * Instances are created and owned by PicoLC and are accessed through members
 * such as PicoLC::StatusLED and PicoLC::FaultLED.
 */
class PicoLCLED {
    friend class PicoLC;

public:
    /** Maximum user-facing brightness percentage. */
    static constexpr uint16_t BRIGHTNESS_MAX = 100;

private:
    /**
     * @brief Constructs an LED associated with an internal channel.
     *
     * PicoLC creates and initializes LED instances during begin().
     *
     * @param pin Internal channel identifier for the LED.
     */
    explicit PicoLCLED(uint8_t pin);

    /** Maximum internal PWM duty representation. */
    static constexpr uint16_t PWM_MAX = 1000;

    /**
     * @brief Advances pulse, blink, and PWM behavior for one scan.
     *
     * This method is called internally by PicoLC::update() using the shared
     * PicoLC scan timestamp.
     *
     * @param currentMillis Current PicoLC scan time in milliseconds.
     */
    void update(uint32_t currentMillis);

    uint32_t _pulseStart = 0;
    uint32_t _pulseLength = 0;
    uint32_t _blinkPeriod = 0;
    uint32_t _blinkStart = 0;
    uint32_t _blinkTarget = 0;
    uint32_t _blinkOnTime = 0;
    uint16_t _brightness = BRIGHTNESS_MAX;
    PatternMode _patternMode = PatternMode::Steady;
    uint8_t _channel;
    bool _state = false;

    PicoLCLED(const PicoLCLED&) = delete;
    PicoLCLED& operator=(const PicoLCLED&) = delete;

public:
    /**
     * @brief Sets the timed behavior applied to the LED.
     *
     * Pulse and blink configuration values are retained when changing modes.
     *
     * @param mode Desired PatternMode.
     */
    void patternMode(PatternMode mode);

    /**
     * @brief Returns the configured timed LED behavior.
     *
     * @return Current PatternMode.
     */
    PatternMode patternMode() const;

    /**
     * @brief Returns the current logical LED state.
     *
     * The returned value never represents the instantaneous physical LED
     * state. In Pulse mode, read() returns false after a pulse is triggered,
     * even while the timed pulse remains active.
     *
     * @return Current logical LED state.
     */
    bool read() const;

    /**
     * @brief Applies a logical command to the LED.
     *
     * In Steady and Blink modes, the supplied value becomes the logical LED
     * state. In Blink mode, a false-to-true transition starts a new cycle in
     * the on phase, and false stops the pattern immediately.
     *
     * In Pulse mode, true starts or restarts the pulse timer and then returns
     * the logical state to false. Writing false stops an active pulse
     * immediately.
     *
     * @param state Logical command to apply.
     * @return The supplied logical command, allowing the call to be chained
     *         in an expression.
     */
    bool write(bool state);

    /**
     * @brief Toggles the LED state or active pulse.
     *
     * In Steady and Blink modes, this is equivalent to writing the opposite
     * logical state. In Pulse mode, an active pulse is stopped; otherwise a
     * new pulse is started.
     */
    void toggle();

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
     * A default value of zero produces no visible output in Blink mode until
     * the on-time is configured.
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
     * @brief Sets the perceived LED brightness.
     *
     * The public range is 0 through BRIGHTNESS_MAX percent. The value is
     * converted internally to a nonlinear PWM duty so that user-facing
     * brightness changes appear more visually uniform.
     *
     * Brightness is applied only during active steady, pulse, or blink phases.
     * A brightness of zero does not change the logical LED state or pattern.
     *
     * @param brightness Brightness percentage from 0 through BRIGHTNESS_MAX.
     */
    void brightness(uint16_t brightness);

    /**
     * @brief Returns the configured perceived LED brightness.
     *
     * The internal PWM representation is converted back to the user-facing
     * percentage scale.
     *
     * @return Brightness percentage from 0 through BRIGHTNESS_MAX.
     */
    uint16_t brightness() const;
};