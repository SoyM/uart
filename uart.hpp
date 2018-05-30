#ifndef INCLUDE_UART_HPP_
#define INCLUDE_UART_HPP_
 
#include <unistd.h> 

#define BUF_SIZE 5

class uart{
    public:
        int _fd;
        uart(const char* uartbus,int baudrate);
        ssize_t set_speed(int speed);
        ssize_t set_Parity(int databits,int stopbits,int parity);
        
        ssize_t uart_read(char *r_buf,size_t len);
        ssize_t uart_write(const char *w_buf,size_t len);
        ssize_t uart_close(int fd);
        ssize_t set_velocity(char velocity_linear,char velocity_angular);
        ssize_t get_velocity();

    private:
        int ret;
        ssize_t safe_read(char* vptr,size_t n);
        ssize_t safe_write(const char *vptr, size_t n);      
};



#endif