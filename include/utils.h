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

enum LogStatus: uint8_t {
    INFO = 0,
    WARNING = 1,
    CRITICAL = 2,
};

inline void LOG(const char *str, LogStatus status) {
    std::cout << str << std::endl;
    switch (status) {
        case LogStatus::CRITICAL: exit(0);
        case LogStatus::WARNING: break;
        case LogStatus::INFO: break;
    }
}