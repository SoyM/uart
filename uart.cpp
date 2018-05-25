#include "uart.hpp"

uart::uart(const char* uartbus,int baudrate){
    char uart_device[64];
    snprintf(uart_device, sizeof(uart_device), "/dev/%s", uartbus);
    //open uart
    _fd = open(uart_device, O_RDWR|O_NOCTTY);  
    if (_fd < 0) {  
        perror(uart_device);  
        exit(1);  
    }  
    //set speed of uart
    if(set_speed(baudrate) != 0){
        perror(uart_device);  
        exit(1);  
    }  
    //set parity of uart
    if (set_Parity(8,1,'N') == -1)  {  
        printf("Set Parity Error\n");  
        exit (0);  
    }
}

////////////////////////////////////////////////////////////////////////////////  
/** 
*@brief  设置串口通信速率 
*@param  fd     类型 int  打开串口的文件句柄 
*@param  speed  类型 int  串口速度 
*@return  int 
*/  
int uart::set_speed(int speed){ 
    int   i;   
    int   status;   
    struct termios   Opt;  
    tcgetattr(_fd, &Opt);   
     
    tcflush(_fd, TCIOFLUSH);       
    cfsetispeed(&Opt, speed);    
    cfsetospeed(&Opt, speed);
    //change now     
    status = tcsetattr(_fd, TCSANOW, &Opt);    
    if  (status != 0) {          
    perror("tcsetattr fd1");    
    return status;       
    }      
    tcflush(_fd,TCIOFLUSH); 
    return status;    
}  


////////////////////////////////////////////////////////////////////////////////  
/** 
*@brief   设置串口数据位，停止位和效验位 
*@param  databits 类型  int 数据位   取值 为 7 或者8 
*@param  stopbits 类型  int 停止位   取值为 1 或者2 
*@param  parity  类型  int  效验类型 取值为N,E,O,,S 
*/  
int uart::set_Parity(int databits,int stopbits,int parity)  
{   
    struct termios options;   
    if  ( tcgetattr(_fd,&options)  !=  0) {   
        perror("SetupSerial 1");       
        return(-1);    
    }  
    options.c_cflag &= ~CSIZE;   
    switch (databits) /*设置数据位数*/  
    {     
    case 7:       
        options.c_cflag |= CS7;   
        break;  
    case 8:       
        options.c_cflag |= CS8;  
        break;     
    default:      
        fprintf(stderr,"Unsupported data size\n"); 
        return (-1);    
    }  
    switch (parity)   
    {     
        case 'n':  
        case 'N':      
            options.c_cflag &= ~PARENB;   /* Clear parity enable */  
            options.c_iflag &= ~INPCK;     /* Enable parity checking */   
            break;    
        case 'o':     
        case 'O':       
            options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/    
            options.c_iflag |= INPCK;             /* Disnable parity checking */   
            break;    
        case 'e':    
        case 'E':     
            options.c_cflag |= PARENB;     /* Enable parity */      
            options.c_cflag &= ~PARODD;   /* 转换为偶效验*/       
            options.c_iflag |= INPCK;       /* Disnable parity checking */  
            break;  
        case 'S':   
        case 's':  /*as no parity*/     
            options.c_cflag &= ~PARENB;  
            options.c_cflag &= ~CSTOPB;break;    
        default:     
            fprintf(stderr,"Unsupported parity\n");      
            return (-1);    
        }    
    /* 设置停止位*/    
    switch (stopbits)  
    {     
        case 1:      
            options.c_cflag &= ~CSTOPB;    
            break;    
        case 2:      
            options.c_cflag |= CSTOPB;    
           break;  
        default:      
             fprintf(stderr,"Unsupported stop bits\n");    
             return (-1);   
    }   
    /* Set input parity option */   
    if (parity != 'n')     
        options.c_iflag |= INPCK;   
    tcflush(_fd,TCIFLUSH);  
    options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/     
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */  
    if (tcsetattr(_fd,TCSANOW,&options) != 0)     
    {   
        perror("SetupSerial 3");     
        return -1;    
    }   
    options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/  
    options.c_oflag  &= ~OPOST;   /*Output*/  
    return 0;    
}  


int uart::set_velocity(char velocity_linear, char velocity_angular){
    u_char u_velocity_linear;
    u_char u_velocity_angular;
    u_velocity_linear = velocity_linear + 128;
    u_velocity_angular = velocity_angular +128;
    std::cout<<"velocity_linear : "<<(int)u_velocity_linear<<std::endl;

    // std::cout<<"velocity_linear : "<<(int)u_velocity_linear<<std::endl;
    char checksum = u_velocity_linear + u_velocity_angular;

    //sprintf 拼接法
    char tx_value[9];
    sprintf(tx_value, "LY%c%c%c", u_velocity_linear, u_velocity_angular, checksum);
    printf("sprintf : ");
    for(int i=0;i<9;i++){
        printf("%c",tx_value[i]);
    }

    //std::string 拼接法 （u_char 拼接不能）
    // std::string const& cc  = std::string("LY") + std::string(&velocity_linear) +std::string(&velocity_angular) + std::string(&checksum);
    // const char * v = cc.c_str();
    // printf("\ntx_data : ");
    // printf("%s",v);

    size_t len = 8;
    int writed_len = uart_write(tx_value, len);
    printf("\nwrite_len : %d \n", writed_len);

    return writed_len;
}

ssize_t uart::safe_read(char* vptr,size_t n)
{
    size_t nleft;
    ssize_t nread;
    char* ptr;

    ptr = vptr;
    nleft = n;

    while(nleft > 0)
    {
        if((nread = read(_fd,ptr,nleft)) < 0)
        {
            if(errno == EINTR)//被信号中断
                nread = 0;
            else
                return -1;
        }
        else
        if(nread == 0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return (n-nleft);
}

int uart::uart_read(char *r_buf,size_t len)
{
    ssize_t cnt = 0;
    fd_set rfds;
    struct timeval time;

    /*将文件描述符加入读描述符集合*/
    FD_ZERO(&rfds);
    FD_SET(_fd,&rfds);

    /*设置超时为15s*/
    time.tv_sec = 15;
    time.tv_usec = 0;

    /*实现串口的多路I/O*/
    ret = select(_fd+1,&rfds,NULL,NULL,&time);
    switch(ret)
    {
        case -1:
            fprintf(stderr,"select error!\n");
            return -1;
        case 0:
            fprintf(stderr,"time over!\n");
            return -1;
        default:
            cnt = safe_read(r_buf,len);
            if(cnt == -1)
            {
                fprintf(stderr,"read error!\n");
                return -1;
            }
            return cnt;
    }
}

ssize_t uart::safe_write(const char *vptr, size_t n)
{
    size_t  nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;

    while(nleft > 0)
    {
    if((nwritten = write(_fd, ptr, nleft)) <= 0)
        {
            if(nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return n;
}

int uart::uart_write(const char *w_buf,size_t len)
{
    ssize_t cnt = 0;

    cnt = safe_write(w_buf,len);
    if(cnt == -1)
    {
        fprintf(stderr,"write error!\n");
        return -1;
    }

    return cnt;
}

int uart::uart_close(int fd)
{
    assert(fd);
    close(fd);

    /*可以在这里做些清理工作*/

    return 0;
}

// volatile char tx_data[] = {crc_value, velocity_angular, velocity_linear, 0xFF};
// //把 8 bit char 组合成 32 bit unsigned int
// uint32_t v;
// for(uint8_t i=0;i<4;i++){
//         v |= ((char)tx_data[3-i]&0xFFu)<<(i*8);  
// }
// // char* v = tx_data;
// uint8_t p1_len = sizeof(v);
// printf("hex: 0x%x len: %u\n", v, p1_len);
