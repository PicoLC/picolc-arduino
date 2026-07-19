#pragma once

#include <Arduino.h>

class PicoLC;

/**
 * @brief Represents one filtered, logical PicoLC button.
 *
 * PicoLCButton samples a board-defined button during PicoLC::update(), applies
 * its configured inversion, then applies the configured stability filter
 * before updating the logical button state. Edge detection operates only on
 * the resulting logical state.
 *
 * Instances are created and owned by PicoLC and are accessed through members
 * such as PicoLC::BootButton, PicoLC::RunStopButton, and PicoLC::UserButton.
 */
class PicoLCButton {
    friend class PicoLC;

private:
    PicoLC& _plc;
    uint8_t _channel;
    bool _lastValue = false;
    bool _currentValue = false;
    uint32_t _filterMillis = 0;
    uint32_t _filterStart = 0;
    bool _candidateValue = false;
    bool _inverted = false;

    /**
     * @brief Constructs a button associated with an internal channel.
     *
     * PicoLC creates and initializes button instances during begin(). The
     * channel value is an opaque, board-specific identifier and should not be
     * interpreted by user code.
     *
     * @param plc PicoLC instance that owns the button and provides scan state.
     * @param channel Internal channel identifier for the button.
     */
    explicit PicoLCButton(PicoLC& plc, uint8_t channel);

    /**
     * @brief Samples and updates the button for one scan.
     *
     * Inversion is applied before filtering. A new logical state is accepted
     * only after the candidate value remains stable for the configured filter
     * duration. Edge state remains valid until the next PicoLC::update().
     *
     * The initial scan synchronizes button state without generating rising,
     * falling, or changed indications.
     *
     * @param currentMillis Current PicoLC scan time in milliseconds.
     */
    void update(uint32_t currentMillis);

    PicoLCButton(const PicoLCButton&) = delete;
    PicoLCButton& operator=(const PicoLCButton&) = delete;

    /**
     * @brief Sets the board-defined logical inversion for the button.
     *
     * Inversion is configured internally by PicoLC and is applied before
     * filtering and edge detection.
     *
     * @param invert true to invert the sampled physical button state.
     */
    void inverted(bool invert);

public:
    /**
     * @brief Returns the current filtered logical button state.
     *
     * The value is true when the button is logically pressed and changes only
     * during PicoLC::update().
     *
     * @return true when the button is logically pressed.
     */
    bool read() const;

    /**
     * @brief Reports a logical button press transition.
     *
     * Filtering and inversion are applied before edge detection. The result
     * remains true for the current scan and is cleared by the next
     * PicoLC::update().
     *
     * @return true when the logical button state changed from released to
     *         pressed during the current scan.
     */
    bool rising() const;

    /**
     * @brief Reports a logical button release transition.
     *
     * Filtering and inversion are applied before edge detection. The result
     * remains true for the current scan and is cleared by the next
     * PicoLC::update().
     *
     * @return true when the logical button state changed from pressed to
     *         released during the current scan.
     */
    bool falling() const;

    /**
     * @brief Reports any logical button transition.
     *
     * Filtering and inversion are applied before change detection. The result
     * remains true for the current scan and is cleared by the next
     * PicoLC::update().
     *
     * @return true when the logical button state changed during the current
     *         scan.
     */
    bool changed() const;

    /**
     * @brief Sets the required button-state stability time.
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
     * @brief Returns the configured button-state stability time.
     *
     * @return Filter duration in milliseconds.
     */
    uint32_t filterMillis() const;
};