#include "uart.hpp"
#include <string.h>  
#include <stdio.h>  
#include <fcntl.h> 

using namespace std;

#define SHOWDEBUG   1
#define BAUDRATE        B57600  
#define UART_DEVICE    "ttyUSB0"


#define BUF_SIZE 255


int main(int argc, char *argv[])  
{   
    //open uart
    int fd, c=0, res; 

    uart uart(UART_DEVICE, BAUDRATE);


    printf("Open...\n"); 

    while(1) {

        int velocity_linear;
        cin>>velocity_linear;
        uart.set_velocity(velocity_linear, 2);
        

        char r_buf[BUF_SIZE+1];
        int ret = read(uart._fd, r_buf, BUF_SIZE);

        // int ret = uart.uart_read(r_buf, 255);
        printf("read form serial:\t len: %lu \t char:",sizeof(r_buf));
        for(char i=0;i<sizeof(r_buf);i++){
            printf("%c",r_buf[i]);
        }
        printf("\n");
    }  
    
    printf("Close...\n");  
    close(fd);  
  
    return 0;  
}  