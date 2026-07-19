#pragma once

#include <Arduino.h>

class PicoLC;

/**
 * @brief Represents one filtered, logical PicoLC input.
 *
 * PicoLCInput samples a physical input during PicoLC::update(), applies
 * optional inversion, then applies the configured stability filter before
 * updating its logical state. Edge detection operates only on the resulting
 * logical state.
 *
 * Instances are created and owned by PicoLC and are accessed through members
 * such as PicoLC::X0 through PicoLC::X3.
 */
class PicoLCInput {
    friend class PicoLC;

private:
    PicoLC &_plc;
    uint8_t _channel;
    bool _lastValue = false;
    bool _currentValue = false;
    uint32_t _filterMillis = 0;
    uint32_t _filterStart = 0;
    bool _candidateValue = false;
    bool _inverted = false;

    /**
     * @brief Constructs an input associated with an internal channel.
     *
     * PicoLC creates and initializes input instances during begin().
     *
     * @param plc Reference to the parent PicoLC instance.
     * @param channel Internal channel identifier for the input.
     */
    PicoLCInput(PicoLC& plc, uint8_t channel);

    /**
     * @brief Samples and updates the input for one scan.
     *
     * Inversion is applied before filtering. A new logical state is accepted
     * only after the candidate value remains stable for the configured filter
     * duration. Edge state remains valid until the next PicoLC::update().
     *
     * The initialization scan synchronizes input state without generating
     * rising, falling, or changed indications.
     *
     * @param currentMillis Current PicoLC scan time in milliseconds.
     */
    void update(uint32_t currentMillis);

    PicoLCInput(const PicoLCInput&) = delete;
    PicoLCInput& operator=(const PicoLCInput&) = delete;

public:
    /**
     * @brief Returns the current filtered logical input state.
     *
     * The value changes only during PicoLC::update().
     *
     * @return Current logical input state.
     */
    bool read() const;

    /**
     * @brief Reports a false-to-true logical transition.
     *
     * Filtering and inversion are applied before edge detection. The result
     * remains true for the current scan and is cleared by the next
     * PicoLC::update().
     *
     * @return true when the logical input rose during the current scan.
     */
    bool rising() const;

    /**
     * @brief Reports a true-to-false logical transition.
     *
     * Filtering and inversion are applied before edge detection. The result
     * remains true for the current scan and is cleared by the next
     * PicoLC::update().
     *
     * @return true when the logical input fell during the current scan.
     */
    bool falling() const;

    /**
     * @brief Reports any logical input transition.
     *
     * Filtering and inversion are applied before change detection. The result
     * remains true for the current scan and is cleared by the next
     * PicoLC::update().
     *
     * @return true when the logical input changed during the current scan.
     */
    bool changed() const;

    /**
     * @brief Sets the required input stability time.
     *
     * A value of zero disables filtering and applies the sampled logical state
     * during the next PicoLC::update(). Changing the duration does not restart
     * an active filter interval; the new value is compared against the
     * existing elapsed interval during the next update.
     *
     * @param ms Required stability time in milliseconds.
     */
    void filterMillis(uint32_t ms);

    /**
     * @brief Returns the configured input stability time.
     *
     * @return Filter duration in milliseconds.
     */
    uint32_t filterMillis() const;

    /**
     * @brief Enables or disables logical input inversion.
     *
     * Inversion is applied before filtering and edge detection. This is a
     * setup-time configuration setting. Behavior is undefined if changed
     * while normal scan processing is active.
     *
     * The new setting takes effect during the next PicoLC::update().
     *
     * @param invert true to invert the sampled physical input.
     */
    void inverted(bool invert);

    /**
     * @brief Returns whether logical input inversion is enabled.
     *
     * @return true when the sampled physical input is inverted.
     */
    bool inverted() const;
};