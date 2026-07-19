#pragma once

/**
 * @brief Selects the electrical drive configuration for a PicoLC output.
 *
 * The output mode determines how the output driver applies logical active and
 * inactive states to the connected load. It is independent of PatternMode,
 * which controls steady, pulse, and blink behavior.
 *
 * Physical output drive remains globally disabled until
 * PicoLC::enableOutputs() is called.
 */
enum class OutputMode : uint8_t {
    /** Drives the load from the positive supply when logically active. */
    HighSide,

    /** Drives the load toward ground when logically active. */
    LowSide,

    /** Actively drives both logical states. */
    PushPull,

    /**
     * Places the output under the control of its associated PicoLCPair.
     *
     * An output in this mode should not be controlled independently.
     */
    Paired,
};

/**
 * @brief Selects the timed behavior applied to a logical output command.
 *
 * Pulse and blink configuration values are retained when changing modes.
 * PWM duty is applied while the output pattern is active.
 */
enum class PatternMode : uint8_t {
    /** The logical state is applied continuously. */
    Steady,

    /**
     * A true command starts or restarts a fixed-duration pulse.
     *
     * The logical state returns to false immediately after the pulse is
     * triggered, while the timed pulse continues internally. Writing false
     * stops an active pulse immediately.
     */
    Pulse,

    /**
     * A true logical state enables a repeating on/off pattern.
     *
     * A false-to-true transition starts a new blink cycle in the on phase.
     * Writing false stops blinking and disables the output immediately.
     */
    Blink
};

enum class PairMode : uint8_t {
    Coast = 0b00,
    Reverse = 0b01,
    Forward = 0b10,
    Brake = 0b11
};

enum class TimerMode : uint8_t {
    TON,
    TOF,
    TP
};

