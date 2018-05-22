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

#define FALSE  -1  
#define TRUE   0 

class uart{
    public:
        void set_speed(int fd, int speed);
        int set_Parity(int fd,int databits,int stopbits,int parity);

};



#endif