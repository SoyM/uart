#include "uart.hpp"
#include <string.h>  
#include <stdio.h>  
#include <fcntl.h> 

using namespace std;

#define SHOWDEBUG   1
#define BAUDRATE        B57600  
#define UART_DEVICE    "ttyUSB0"


int main()  
{   
    uart uart(UART_DEVICE, BAUDRATE);

    printf("Open...\n"); 

    int count = 0;
    int count_2 = 0;



    for(char m = -128; m < 127; m++){
        for(char n = -128; n < 127; n++){
            cout<<"loop : "<< count <<"\tsuccess :" << count_2 <<endl;
            uart.set_velocity(m, n);
            int get_velocity = uart.get_velocity();
            if(get_velocity != 257){
                count_2++;
            }
            cout<<"get_velocity: "<< get_velocity <<endl;
            count++;

            // struct timeval delay;  
            // delay.tv_sec = 0;  
            // delay.tv_usec = 10*1000 ; // 10 ms  
            // select(0, NULL, NULL, NULL, &delay);
        }
    }

    printf("Close...\n");  
    return 0;  
}  