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

#define FALSE  -1  
#define TRUE   0 

class uart{
    public:
        void set_speed(int fd, int speed);
        int set_Parity(int fd,int databits,int stopbits,int parity);
        ssize_t safe_read(int fd,char* vptr,size_t n);
        ssize_t safe_write(int fd, const char *vptr, size_t n);
        int uart_read(int fd,char *r_buf,size_t len);
        int uart_write(int fd,const char *w_buf,size_t len);
        int uart_close(int fd);

};



#endif