#include <termios.h>  
#include <stdio.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#include "crc8.hpp"
#include "uart.hpp"

using namespace std;

#define BAUDRATE        B115200  
#define UART_DEVICE     "/dev/ttyUSB0"  
  
#define FALSE  -1  
#define TRUE   0

  
int main(int argc, char *argv[])  
{  
    crc8 crc;
    uart uart;

    int fd, c=0, res;   
    uint8_t crcTable8bit[256];
    
    uint8_t polynominal8bit;
    uint8_t initValue;
    uint8_t xorValue;

    polynominal8bit = 0x2F;
    initValue = 0xFF;
    xorValue = 0xFF;

    crc.Crc8TableGenerator(polynominal8bit, crcTable8bit);
  
    printf("Start...\n");  
    fd = open(UART_DEVICE, O_RDWR);  
  
    if (fd < 0) {  
        perror(UART_DEVICE);  
        exit(1);  
    }  
  
    printf("Open...\n");  
    uart.set_speed(fd,2000000);  
    if (uart.set_Parity(fd,8,1,'N') == FALSE)  {  
        printf("Set Parity Error\n");  
        exit (0);  
    }  
    printf("loop...\n");  

    while(1) {
         
        char velocity_linear = 0x01;
        char velocity_angular = 0x02;
        u_int16_t* velocity_la;
        u_int16_t velocity_la_value = ((char)velocity_angular & 0xFF) | (((char)velocity_linear & 0xFF)<<8);
        velocity_la = &velocity_la_value; 
        

        uint8_t message1[] = {velocity_linear, velocity_angular};
        uint8_t crc_value = crc.CalculateCRC8(crcTable8bit, message1, sizeof(message1), initValue, xorValue, false, false);
        printf("origin: %x, crc: %x\n", *velocity_la, crc_value);


        char tx_data[] = {0xFF , velocity_linear , velocity_angular , crc_value};


        // uint32_t data_mix;
        // for(uint8_t i=0;i<4;i++){
        //         data_mix |= ((char)tx_data[3-i]&0xFFu)<<(i*8);  
        // }
        // char* v = tx_data;
        // char p1_len = strlen(v);
        // printf("hex: 0x%x len: %u\n", *v, p1_len);

        for(uint8_t i=0;i < sizeof(tx_data);i++){
            uint8_t n;
            n=write(fd, &(tx_data[i]), 1);

            printf("write : %x   write_len : %d \n",tx_data[i], n);
            // perror("Failed to write to the output\n");
  
        }

        char buf[4];
        res = read(fd, buf, 4);  
  
        if(res==0)  
            continue;  
        buf[res]=0;  
  
        printf("data from serial : %x", *buf);  
          
        if (buf[0] == 0x0d)  
            printf("\n");  
          
        if (buf[0] == '@') break;  
    }  
    
    printf("Close...\n");  
    close(fd);  
  
    return 0;  
}  