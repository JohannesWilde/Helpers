#ifndef BYTE_SWAP_HPP
#define BYTE_SWAP_HPP

#include <stdint.h>

namespace Helpers
{

uint8_t byteSwap(uint8_t const value)
{
    return value;
}

uint16_t byteSwap(uint16_t const value)
{
    uint16_t workValue = value;
    {
        uint16_t constexpr mask = 0xff00;
        workValue = ((workValue & mask) >> 8) | ((workValue & ~mask) << 8);
    }
    return workValue;
}

uint32_t byteSwap(uint32_t const value)
{
    uint32_t workValue = value;
    {
        uint32_t constexpr mask = 0xffff0000;
        workValue = ((workValue & mask) >> 16) | ((workValue & ~mask) << 16);
    }
    {
        uint32_t constexpr mask = 0xff00ff00;
        workValue = ((workValue & mask) >> 8) | ((workValue & ~mask) << 8);
    }
    return workValue;
}

uint64_t byteSwap(uint64_t const value)
{
    uint64_t workValue = value;
    {
        uint64_t constexpr mask = 0xffffffff00000000;
        workValue = ((workValue & mask) >> 32) | ((workValue & ~mask) << 32);
    }
    {
        uint64_t constexpr mask = 0xffff0000ffff0000;
        workValue = ((workValue & mask) >> 16) | ((workValue & ~mask) << 16);
    }
    {
        uint64_t constexpr mask = 0xff00ff00ff00ff00;
        workValue = ((workValue & mask) >> 8) | ((workValue & ~mask) << 8);
    }
    return workValue;
}

} // namespace Helpers

#endif // BYTE_SWAP_HPP
