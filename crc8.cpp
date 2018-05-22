#include "crc8.hpp"

//This source code is helper for implementing CRC by AutoSar documentation: Specification of CRC Routines.
// https://www.autosar.org/fileadmin/files/standards/classic/

// Good page for checking your CRC:
// http://www.sunshine2k.de/coding/javascript/crc/crc_js.html


void crc8::Crc8TableGenerator(uint8_t polynomial, uint8_t crcTable[256])
{
    uint8_t remainder;

    uint8_t topBit = 0x80;
    uint32_t ui32Dividend;

    for (ui32Dividend = 0; ui32Dividend < 256; ui32Dividend++)
    {
        remainder = ui32Dividend;

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (0 == (remainder & topBit))
            {
                remainder <<= 1;
            }
            else
            {
                remainder = (remainder << 1) ^ polynomial;
            }
        }

        crcTable[ui32Dividend] = remainder;

        //remove comments if you want print CRC 256 lookup table 
        /*
        if (0 == dividend % 8)
        {
            printf("\n");
        }
        printf("0x%02xU, ", remainder);
        */

    }
}

uint8_t crc8::CalculateCRC8(uint8_t crcTable[256], const uint8_t *crc_DataPtr, uint32_t crc_Length, uint8_t crc_InitialValue, uint8_t crc_XorValue, bool reflectedOutput, bool reflectedInput)
{
    uint32_t ui32Counter;
    uint8_t temp;
    uint8_t crc = crc_InitialValue;

    for (ui32Counter = 0U; ui32Counter < crc_Length; ui32Counter++)
    {
        if (reflectedInput)
        {
            temp = reflect(*crc_DataPtr, 8);
        }
        else
        {
            temp = *crc_DataPtr;
        }

        crc = crc ^ temp;
        crc = crcTable[crc];
        crc_DataPtr++;
    }

    crc ^= crc_XorValue;
    if (reflectedOutput)
    {
        crc = reflect(crc, 8);
    }
    return crc;
}

void crc8::TestCRC8(uint8_t calculatedCrc, uint8_t expectedCrc)
{
    if (expectedCrc != calculatedCrc)
    {
        printf("Error for CRC8 0x%x\n", calculatedCrc);
    }
    else
    {
        printf("CRC8 0x%02x OK!\n", calculatedCrc);
    }
}

uint32_t crc8::reflect(uint32_t data, uint8_t nBits)
{
    uint32_t  reflection = 0;
    /*
    * Reflect the data about the center bit.
    */
    for (uint8_t bit = 0; bit < nBits; ++bit)
    {
        /*
        * If the LSB bit is set, set the reflection of it.
        */
        if (data & 0x01)
        {
            reflection |= (1 << ((nBits - 1) - bit));
        }

        data = (data >> 1);
    }

    return (reflection);

}   

/*
int main(){
    while(1){
    uint8_t crcTable8bit[256];

    uint8_t polynominal8bit;
    uint8_t initValue;
    uint8_t xorValue;

    uint8_t message1[] = { 0x00, 0x00, 0x00, 0x00 };
    uint8_t message2[] = { 0xF2, 0x01, 0x83 };
    uint8_t message3[] = { 0x0F, 0xAA, 0x00, 0x55 };
    uint8_t message4[] = { 0x00, 0xFF, 0x55, 0x11 };
    uint8_t message5[] = { 0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0XFF };
    uint8_t message6[] = { 0x92, 0x6B, 0x55 };
    uint8_t message7[] = { 0xFF, 0xFF, 0xFF, 0xFF };

    polynominal8bit = 0x2F;
    initValue = 0xFF;
    xorValue = 0xFF;
    Crc8TableGenerator(polynominal8bit, crcTable8bit);

    TestCRC8(CalculateCRC8(crcTable8bit, message1, sizeof(message1), initValue, xorValue, false, false), 0x12);
    TestCRC8(CalculateCRC8(crcTable8bit, message2, sizeof(message2), initValue, xorValue, false, false), 0xC2);
    TestCRC8(CalculateCRC8(crcTable8bit, message3, sizeof(message3), initValue, xorValue, false, false), 0xC6);
    TestCRC8(CalculateCRC8(crcTable8bit, message4, sizeof(message4), initValue, xorValue, false, false), 0x77);
    TestCRC8(CalculateCRC8(crcTable8bit, message5, sizeof(message5), initValue, xorValue, false, false), 0x11);
    TestCRC8(CalculateCRC8(crcTable8bit, message6, sizeof(message6), initValue, xorValue, false, false), 0x33);
    TestCRC8(CalculateCRC8(crcTable8bit, message7, sizeof(message7), initValue, xorValue, false, false), 0x6C);
    };
    return 0;
}
*/