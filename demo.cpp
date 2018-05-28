#include "uart.hpp"
#include <string.h>  
#include <stdio.h>  
#include <thread>
#include <fcntl.h> 

using namespace std;

#define SHOWDEBUG   1UI
#define BAUDRATE        B57600  
#define UART_DEVICE    "ttyUSB0"

void read_uart(){
    uart uart(UART_DEVICE, BAUDRATE);
    while(1){
        cout<<"get_velocity: "<<uart.get_velocity()<<endl;
    }
}


int main()  
{   
    thread th1(read_uart);
    // th1.join();

    uart uart(UART_DEVICE, BAUDRATE);

    printf("Open...\n"); 

    
    for(char m = -128; m < 127; m++){
        for(char n = -128; n < 127; n++){
                uart.set_velocity(m, n);
        }
    }

    printf("Close...\n");  
    return 0;  
}  