#ifndef INCLUDE_UART_HPP_
#define INCLUDE_UART_HPP_

#include <sys/types.h>  
#include <stdlib.h>
#include <sys/stat.h>  
#include <fcntl.h>  
#include <termios.h>  
#include <stdio.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#include <bitset>
#include<errno.h>
#include<assert.h>

class uart{
    public:
        int _fd;
        uart(const char* uartbus,int baudrate);
        int set_speed(int speed);
        int set_Parity(int databits,int stopbits,int parity);
        int uart_read(char *r_buf,size_t len);
        int uart_write(const char *w_buf,size_t len);
        int uart_close(int fd);
        int set_velocity(char velocity_linear,char velocity_angular);

    private:
        int ret;
        ssize_t safe_read(char* vptr,size_t n);
        ssize_t safe_write(const char *vptr, size_t n);      
};



#endif