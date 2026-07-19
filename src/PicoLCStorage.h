#pragma once

#include <Arduino.h>

class PicoLCStorage {
    friend class PicoLC;

private:
    PicoLCStorage();

public:
    size_t length() const;  // 4096

    uint8_t read(uint32_t address) const;
    bool write(uint32_t address, uint8_t value);
    bool update(uint32_t address, uint8_t value);

    bool read(uint32_t address, void* data, size_t length) const;
    bool write(uint32_t address, const void* data, size_t length);

    template <typename T>
    bool get(uint32_t address, T& value) const {
        return read(address, &value, sizeof(T));
    }

    template <typename T>
    bool put(uint32_t address, const T& value) {
        return write(address, &value, sizeof(T));
    }
};

