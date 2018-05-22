#ifndef INCLUDE_CRC8_HPP_
#define INCLUDE_CRC8_HPP_

#include <stdio.h>
#include <stdint.h>

class crc8
{
    public:
        void Crc8TableGenerator(uint8_t polynomial, uint8_t crcTable[256]);
        uint8_t CalculateCRC8(uint8_t crcTable[256], const uint8_t *crc_DataPtr, uint32_t crc_Length, uint8_t crc_InitialValue, uint8_t crc_XorValue, bool reflectedOutput, bool reflectedInput);
        void TestCRC8(uint8_t calculatedCrc, uint8_t expectedCrc);

    private:
        uint32_t reflect(uint32_t data, uint8_t nBits);
        
};

#endif