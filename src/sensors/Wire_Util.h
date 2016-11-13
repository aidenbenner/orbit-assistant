#ifndef WIRE_UTIL_H
#define WIRE_UTIL_H
void WireInit();
void WireWriteByte(int address, uint8_t value);
void WireWriteRegister(int address, uint8_t reg, uint8_t value);
void WireRequestArray(int address, uint32_t* buffer, uint8_t amount);
#endif
