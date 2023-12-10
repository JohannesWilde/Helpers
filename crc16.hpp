#ifndef CRC16_HPP
#define CRC16_HPP

#include "bitSwap.hpp"

#include <limits.h>
#include <stdint.h>

static_assert(8 == CHAR_BIT);


namespace Internal_
{

template <typename T, bool reflect>
class BitSwap
{
public:
    static T of(T const value);

private:
    BitSwap() = delete;
};


template <typename T>
class BitSwap<T, false>
{
public:
    static T of(T const value)
    {
        return value;
    }

private:
    BitSwap() = delete;
};


template <typename T>
class BitSwap<T, true>
{
public:
    static T of(T const value)
    {
        return Helpers::bitSwap(value);
    }

private:
    BitSwap() = delete;
};

} // namespace Internal_



template <uint16_t polynomial,
         uint16_t initialCrc,
         bool reflectIn,
         bool reflectOut,
         uint16_t xorOut>
class Crc16
{
public:

    Crc16() noexcept
        : crc_(initialCrc)
    {
    }

    void process(uint8_t const * const data, size_t const octectCount) noexcept
    {
        for (uint8_t const * datumPointer = data; (data + octectCount) > datumPointer; ++datumPointer)
        {
            // Include the new data in the CRC calculation by virtually appending it to accumulated
            // and already processed data. Now make it represented in the CRC calculation by the
            // XOR operation [i.e. simply pretend, this datum has always been here and is thus reflected
            // in the remainder of the previous calculation already].
            // Do so with the upper byte, as to conform to the notion of "appending 16 bits of 0 for the
            // calculation" [8 bits shifted here, 8 bits shifted in below loop] - so as to perform the
            // XOR until the last bit of data and really only retain the remainder. Which is what the CRC
            // is supposed to be.
            crc_ ^= (static_cast<uint16_t>(
                         Internal_::BitSwap<uint8_t, reflectIn>::of(*datumPointer)
                         ) << 8);

            for (int bitIndex = 0; bitIndex < CHAR_BIT; ++bitIndex)
            {
                // Perform XOR only if the MSb [Most Significant bit] is set [as the CRC is "[...] the
                // remainder of a polynomial division, modulo two.", Jack Crenshaw's "Implementing CRCs"
                // article in the January 1992 issue of Embedded Systems Programming].
                bool const applyPolynomial = (0 != (0x8000 & crc_));

                // In the polynomial the MSb is not encoded and instead assumed to always be 1 [otherwise
                // the 16-order polynomial would have required 17-bits, which would exceed the value type].
                // So:
                //  - If we are going to apply the polynomial, we already know that 0 == (1 ^ 1) and thus
                //    we disregard this bit.
                //  - If we are not going to apply the polynomial, the bit was 0 and is simply discarded
                //    as well.
                // So in any case, simply shift out the MSb.
                crc_ <<= 1;

                if (applyPolynomial)
                {
                    crc_ ^= polynomial;
                }
            }
        }
    }

    uint16_t get() const noexcept
    {
        return (xorOut ^ Internal_::BitSwap<uint16_t, reflectOut>::of(crc_));
    }

private:

    uint16_t crc_;

};


// https://reveng.sourceforge.io/crc-catalogue/16.htm#crc.cat.crc-16-xmodem
// CRC-16/XMODEM
// width=16 poly=0x1021 init=0x0000 refin=false refout=false xorout=0x0000 check=0x31c3 residue=0x0000 name="CRC-16/XMODEM"
// Class: attested
// Alias: CRC-16/ACORN, CRC-16/LTE, CRC-16/V-41-MSB, XMODEM, ZMODEM
// The MSB-first form of the V.41 algorithm. For the LSB-first form see CRC-16/KERMIT. CRC presented high byte first.
// Used in the MultiMediaCard interface. In XMODEM and Acorn MOS the message bits are processed out of transmission order,
// compromising the guarantees on burst error detection.
// ITU-T Recommendation V.41 (November 1988)

typedef Crc16<0x1021, 0x0000, false, false, 0x0000> Crc16Xmodem;


// https://reveng.sourceforge.io/crc-catalogue/16.htm#crc.cat.crc-16-kermit
// CRC-16/KERMIT
// width=16 poly=0x1021 init=0x0000 refin=true refout=true xorout=0x0000 check=0x2189 residue=0x0000 name="CRC-16/KERMIT"
// Class: attested
// Alias: CRC-16/BLUETOOTH, CRC-16/CCITT, CRC-16/CCITT-TRUE, CRC-16/V-41-LSB, CRC-CCITT, KERMIT
// Used in Bluetooth error detection. Init=0x0000 is used in the Inquiry Response substate.
// Press et al. identify the CCITT algorithm with the one implemented in Kermit. V.41 is endianness-agnostic, referring
// only to bit sequences, but the CRC appears reflected when used with LSB-first modems. Ironically, the unreflected form
// is used in CRC-16/XMODEM.

typedef Crc16<0x1021, 0x0000, true, true, 0x0000> Crc16Kermit;


// https://reveng.sourceforge.io/crc-catalogue/16.htm#crc.cat.crc-16-ibm-3740
// CRC-16/IBM-3740
// width=16 poly=0x1021 init=0xffff refin=false refout=false xorout=0x0000 check=0x29b1 residue=0x0000 name="CRC-16/IBM-3740"
// Class: attested
// Alias: CRC-16/AUTOSAR, CRC-16/CCITT-FALSE
// An algorithm commonly misidentified as CRC-CCITT. CRC-CCITT customarily refers to the LSB-first form of the algorithm in
// ITU-T Recommendation V.41 (see CRC-16/KERMIT); its MSB-first counterpart is CRC-16/XMODEM.
// AUTOSAR (24 November 2022), AUTOSAR Classic Platform release R22-11, Specification of CRC Routines

typedef Crc16<0x1021, 0xffff, false, false, 0x0000> Crc16Ibm3740;


// https://reveng.sourceforge.io/crc-catalogue/16.htm#crc.cat.crc-16-spi-fujitsu
// CRC-16/SPI-FUJITSU
// width=16 poly=0x1021 init=0x1d0f refin=false refout=false xorout=0x0000 check=0xe5cc residue=0x0000 name="CRC-16/SPI-FUJITSU"
// Class: attested
// Alias: CRC-16/AUG-CCITT
// Init value is equivalent to an augment of 0xFFFF prepended to the message.
// Fujitsu Semiconductor (10 October 2007), FlexRay ASSP MB88121B User's Manual (courtesy of the Internet Archive)

typedef Crc16<0x1021, 0x1d0f, false, false, 0x0000> Crc16SpiFujitsu;


#endif // CRC16_HPP
