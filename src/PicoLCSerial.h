#pragma once

#include <Arduino.h>

/**
 * @brief Provides Arduino Stream-compatible access to the PicoLC RS-485 port.
 *
 * PicoLCSerial wraps the hardware UART and RS-485 transmit/receive control
 * pins. The current implementation is blocking: write operations do not
 * return until the requested data has been transmitted and the interface has
 * returned to receive mode.
 *
 * Instances are created and owned by PicoLC.
 */
class PicoLCSerial : public Stream {
    friend class PicoLC;

private:
    PicoLCSerial();

    bool _started = false;

    /**
     * @brief Selects the RS-485 transceiver direction.
     *
     * @param writing true to enable transmission; false to enable reception.
     */
    void setTR(bool writing);

public:
    /**
     * @brief Initializes the RS-485 UART and transceiver-control pins.
     *
     * Calling begin() after end() reinitializes the UART and associated pins.
     *
     * @param baud Serial baud rate.
     * @param config Arduino serial framing configuration.
     */
    void begin(uint32_t baud, uint16_t config = SERIAL_8N1);

    /**
     * @brief Releases PicoLC control of the UART and transceiver-control pins.
     *
     * Received data already present in the UART receive buffer is retained.
     * A later call to begin() reinitializes the interface.
     */
    void end();

    /**
     * @brief Returns the number of received bytes available to read.
     *
     * @return Number of available bytes, or 0 when the interface is not
     *         started.
     */
    int available() override;

    /**
     * @brief Reads the next received byte.
     *
     * @return The next byte as an unsigned value, or -1 when no data is
     *         available or the interface is not started.
     */
    int read() override;

    /**
     * @brief Returns the next received byte without removing it.
     *
     * @return The next byte as an unsigned value, or -1 when no data is
     *         available or the interface is not started.
     */
    int peek() override;

    /**
     * @brief Transmits one byte over RS-485.
     *
     * This operation is blocking and returns after transmission completes and
     * the transceiver has returned to receive mode.
     *
     * @param byte Byte to transmit.
     * @return Number of bytes written, or 0 when the interface is not started.
     */
    size_t write(uint8_t byte) override;

    /**
     * @brief Transmits a buffer over RS-485.
     *
     * This operation is blocking and returns after the complete buffer has
     * been transmitted and the transceiver has returned to receive mode.
     *
     * @param data Pointer to the data to transmit.
     * @param length Number of bytes to transmit.
     * @return Number of bytes written, or 0 when the interface is not started.
     */
    size_t write(const uint8_t* data, size_t length) override;

    /**
     * @brief Waits for pending UART transmission to complete.
     *
     * This method wraps the underlying Arduino serial implementation. It does
     * nothing when the interface is not started.
     */
    void flush() override;

    /**
     * @brief Reports whether the RS-485 interface is initialized.
     *
     * @return true after begin() and until end() is called.
     */
    bool started() const;

    using Print::write;
};