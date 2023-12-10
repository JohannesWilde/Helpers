#ifndef BIT_SWAP_HPP
#define BIT_SWAP_HPP

#include <cstdint>

namespace Helpers
{

uint8_t bitSwap(uint8_t const value)
{
    uint8_t workValue = value;
    {
        uint8_t constexpr mask = 0xf0;
        workValue = ((workValue & mask) >> 4) | ((workValue & ~mask) << 4);
    }
    {
        uint8_t constexpr mask = 0xcc;
        workValue = ((workValue & mask) >> 2) | ((workValue & ~mask) << 2);
    }
    {
        uint8_t constexpr mask = 0xaa;
        workValue = ((workValue & mask) >> 1) | ((workValue & ~mask) << 1);
    }
    return workValue;
}

uint16_t bitSwap(uint16_t const value)
{
    uint16_t workValue = value;
    {
        uint16_t constexpr mask = 0xff00;
        workValue = ((workValue & mask) >> 8) | ((workValue & ~mask) << 8);
    }
    {
        uint16_t constexpr mask = 0xf0f0;
        workValue = ((workValue & mask) >> 4) | ((workValue & ~mask) << 4);
    }
    {
        uint16_t constexpr mask = 0xcccc;
        workValue = ((workValue & mask) >> 2) | ((workValue & ~mask) << 2);
    }
    {
        uint16_t constexpr mask = 0xaaaa;
        workValue = ((workValue & mask) >> 1) | ((workValue & ~mask) << 1);
    }
    return workValue;
}

uint32_t bitSwap(uint32_t const value)
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
    {
        uint32_t constexpr mask = 0xf0f0f0f0;
        workValue = ((workValue & mask) >> 4) | ((workValue & ~mask) << 4);
    }
    {
        uint32_t constexpr mask = 0xcccccccc;
        workValue = ((workValue & mask) >> 2) | ((workValue & ~mask) << 2);
    }
    {
        uint32_t constexpr mask = 0xaaaaaaaa;
        workValue = ((workValue & mask) >> 1) | ((workValue & ~mask) << 1);
    }
    return workValue;
}

uint64_t bitSwap(uint64_t const value)
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
    {
        uint64_t constexpr mask = 0xf0f0f0f0f0f0f0f0;
        workValue = ((workValue & mask) >> 4) | ((workValue & ~mask) << 4);
    }
    {
        uint64_t constexpr mask = 0xcccccccccccccccc;
        workValue = ((workValue & mask) >> 2) | ((workValue & ~mask) << 2);
    }
    {
        uint64_t constexpr mask = 0xaaaaaaaaaaaaaaaa;
        workValue = ((workValue & mask) >> 1) | ((workValue & ~mask) << 1);
    }
    return workValue;
}

} // namespace Helpers

#endif // BIT_SWAP_HPP
