#include "PicoLC.h"

#include "PicoLCStorage.h"

PicoLCStorage::PicoLCStorage() {}
size_t PicoLCStorage::length() const { return 4096; }
uint8_t PicoLCStorage::read(uint32_t) const { return 0; }
bool PicoLCStorage::write(uint32_t, uint8_t) { return false; }
bool PicoLCStorage::update(uint32_t, uint8_t) { return false; }
bool PicoLCStorage::read(uint32_t, void*, size_t) const { return false; }
bool PicoLCStorage::write(uint32_t, const void*, size_t) { return false; }
