#include "crc8.hpp"
#include "uart.hpp"
#include <signal.h>

using namespace std;

#define BAUDRATE        9600  
#define UART_DEVICE     "/dev/ttyUSB0"  

#define FALSE  -1  
#define TRUE   0

#define BUF_SIZE 4

volatile sig_atomic_t flag = 0;

void my_function(int sig){ // can be called asynchronously
  flag = 1; // set flag
  return ;
}


int main(int argc, char *argv[])  
{  
    crc8 crc;
    uart uart;
    signal(SIGINT, my_function);   
    //
    //Crc8 Table Generator
    //
    uint8_t crcTable8bit[256];
    uint8_t  polynominal8bit = 0x2F;
    uint8_t initValue = 0xFF;
    uint8_t xorValue = 0xFF;

    volatile static int ret;
    char r_buf[BUF_SIZE];

    crc.Crc8TableGenerator(polynominal8bit, crcTable8bit);
    //
    //open serial and set_Parity
    //
    int fd, c=0, res; 
    fd = open(UART_DEVICE, O_RDWR);  
    if (fd < 0) {  
        perror(UART_DEVICE);  
        exit(1);  
    }  
    printf("Open...\n");  
    uart.set_speed(fd, BAUDRATE);  
    if (uart.set_Parity(fd,8,1,'N') == FALSE)  {  
        printf("Set Parity Error\n");  
        exit (0);  
    }  
    //
    //loop
    //
    printf("looping...\n");  
    while(1) {
        //
        //crc velocity_linear and velocity_angular
        //
        volatile char velocity_linear = 0x01;
        volatile char velocity_angular = 0x02;
        uint8_t crc_value;
        u_int16_t* velocity_la;
        u_int16_t velocity_la_value = ((char)velocity_angular & 0xFF) | (((char)velocity_linear & 0xFF)<<8);
        velocity_la = &velocity_la_value; 
        
        uint8_t message1[] = {velocity_linear, velocity_angular};
        crc_value = crc.CalculateCRC8(crcTable8bit, message1, sizeof(message1), initValue, xorValue, false, false);
        printf("origin: %x, crc: %x\n", *velocity_la, crc_value);

        crc_value = velocity_linear + velocity_angular;
        ///////////////////////////////////////////////////////////
        //write data to serial
        ///////////////////////////////////////////////////////////
        volatile char tx_data[] = {0xFF , velocity_linear , velocity_angular , crc_value};

        uint32_t v;
        for(uint8_t i=0;i<4;i++){
                v |= ((char)tx_data[3-i]&0xFFu)<<(i*8);  
        }
        // char* v = tx_data;
        uint8_t p1_len = sizeof(v);
        printf("hex: 0x%x len: %u\n", v, p1_len);

        // for(uint8_t i=0;i < sizeof(tx_data);i++){
        char n;
        n = write(fd, &v, 4);

        printf("write : %x   write_len : %d \n",(int)v, n);
            // perror("Failed to write to the output\n");
        // }


        ////////////////////////////////////////////////////////////////
        //read data from serial
        ////////////////////////////////////////////////////////////////
        bzero(r_buf,BUF_SIZE);

        ret = uart.uart_read(fd,r_buf,BUF_SIZE);
        if(ret == -1)
        {
            fprintf(stderr,"uart read failed!\n");
            exit(EXIT_FAILURE);
        }

        //把 8 bit char 组合成 32 bit unsigned int
        volatile u_int32_t read_v;
        for(uint8_t i=0;i<4;i++){
                read_v |= ((char)r_buf[3-i]&0xFFu)<<(i*8);  
        }
        p1_len = sizeof(read_v);


        printf("\n-----[ read ]------\n");
        printf("-- hex: 0x%x , char: %s , len: %u\n", read_v, r_buf, ret);
        printf("---------------------\n");

        if(r_buf[1]+r_buf[2]==r_buf[3]){
            printf("checksum success\n");
        }

        // char quotient[100]; 
        // char remainder[100];
        // char strP[]= "00101111";
        // char strQ[]="000000010000001000011101";
        // crc.strm2div(strQ, strP, quotient, remainder);
        // printf("quotient: %d, remainder: %d\n",quotient,remainder);
    }  
    
    printf("Close...\n");  
    close(fd);  
  
    return 0;  
}  