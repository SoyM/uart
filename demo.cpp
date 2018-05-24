#include "uart.hpp"
#include <string.h>  
#include <stdio.h>  
#include <fcntl.h> 

using namespace std;

#define SHOWDEBUG   1
#define BAUDRATE        B115200  
#define UART_DEVICE     "/dev/ttyUSB0"  

#define FALSE  -1  
#define TRUE   0

#define BUF_SIZE 8



int main(int argc, char *argv[])  
{   
    uart uart;
    ////////////////////////////////////////////////////
    //init serial and set_Parity
    int fd, c=0, res; 
    fd = open(UART_DEVICE, O_RDWR|O_NOCTTY);  
    if (fd < 0) {  
        perror(UART_DEVICE);  
        exit(1);  
    }  
    uart.set_speed(fd, BAUDRATE);  
    if (uart.set_Parity(fd,8,1,'N') == FALSE)  {  
        printf("Set Parity Error\n");  
        exit (0);  
    }
    printf("Open...\n"); 


    while(1) {
        ////////////////////////////////////////////////////
        //input
        
        // char v[9];
        // printf("--------\n");
        // printf("\tinput:");  
        // cin.getline(v,9);
        // printf("\n"); 
        // for(char i=0;i<8;i++){
        //     printf("%c",v[i]);

        // }

        int velocity_linear = 1;
        int velocity_angular = 2;

        uart.set_velocity(velocity_linear, velocity_angular);
        
        ////////////////////////////////////////////////////////////////
        //read data from serial
        ////////////////////////////////////////////////////////////////
        // bzero(r_buf,BUF_SIZE);

        char r_buf[256];
        int ret = read(fd, r_buf, 255);
        if(ret == -1){
            printf("fuckkkkkkkk");
            if(errno == EINTR){
                close(fd);
            }else if(errno == EAGAIN){
                // 根据你和调用者的约定, 返回一个数值告诉它再次重试
                // 一般是结合 select/epoll 等 IO 多路复用函数
            }
            exit(-1);
        }else if(ret == 0){
            close(fd);
        }
        printf("read form serial:\t len: %lu \t char:",sizeof(r_buf));
        for(char i=0;i<sizeof(r_buf);i++){
            printf("%c",r_buf[i]);
        }
        printf("\n");

        // char ret = uart.uart_read(fd,r_buf,BUF_SIZE);
        // if(ret == -1)
        // {
        //     fprintf(stderr,"uart read failed!\n");
        //     exit(EXIT_FAILURE);
        // }
        // printf("\n read form serial:%c\n",(int)*r_buf);
    }  
    
    printf("Close...\n");  
    close(fd);  
  
    return 0;  
}  