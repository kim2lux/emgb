#pragma once
#include <stdint.h>

inline bool isBitSet(const uint8_t byte, const uint8_t position)
{
    return ((byte & (1 << position)) != 0);
}

inline void setBit(uint8_t & byte, const uint8_t position)
{
    byte = byte | (1 << position);
}

inline void clearBit(uint8_t & byte, const uint8_t position)
{
    byte = byte & ~(1 << position);
}
