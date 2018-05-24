#ifndef INCLUDE_CRC8_HPP_
#define INCLUDE_CRC8_HPP_

#include <stdio.h>
#include <stdint.h>
#include <string.h> 
#include <iostream>

#define SHOWPROCESS 1

class crc8
{
    public:
        void Crc8TableGenerator(uint8_t polynomial, uint8_t crcTable[256]);
        uint8_t CalculateCRC8(uint8_t crcTable[256], const uint8_t *crc_DataPtr, uint32_t crc_Length, uint8_t crc_InitialValue, uint8_t crc_XorValue, bool reflectedOutput, bool reflectedInput);
        void TestCRC8(uint8_t calculatedCrc, uint8_t expectedCrc);
        void strm2div(const char *strM, const char *strP, char *strQ, char *strR);

    private:
        uint32_t reflect(uint32_t data, uint8_t nBits);
        void repeat(char c, int n);
        
};

#endif

    ///////////////////////////////////////////////////////////////////
    //Crc8 Table Generator
    ///////////////////////////////////////////////////////////////////
    // crc8 crc;
    // uint8_t crcTable8bit[256];
    // uint8_t  polynominal8bit = 0x2F;
    // uint8_t initValue = 0xFF;
    // uint8_t xorValue = 0xFF;
    // volatile static int ret;
    // char r_buf[BUF_SIZE];
    // crc.Crc8TableGenerator(polynominal8bit, crcTable8bit);

    ///////////////////////////////////////////////////////////////////
    //calculate value of crc with velocity_linear and velocity_angular
    ///////////////////////////////////////////////////////////////////
    // uint8_t crc_value;
    // u_int16_t* velocity_la;
    // u_int16_t velocity_la_value = ((char)velocity_angular & 0xFF) | (((char)velocity_linear & 0xFF)<<8);
    // velocity_la = &velocity_la_value; 

    // uint8_t message1[] = {velocity_linear, velocity_angular};
    // crc_value = crc.CalculateCRC8(crcTable8bit, message1, sizeof(message1), initValue, xorValue, false, false);
    // printf("origin: %x, crc: %x\n", *velocity_la, crc_value);