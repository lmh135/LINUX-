#ifndef __UART_H__
#define __UART_H__
#include <fcntl.h>  //open();
#include <stdio.h>
#include <string.h>    // memset(),bzero      
#include <unistd.h>
#include <sys/time.h>  //gettimeofday();
#include <sys/ioctl.h>
#include <termios.h>
#include <iostream>
using namespace std;
#define GPS_BUF_NUM 250
extern int ggacount,vtgcount,headingcount;
extern bool gga_update_flag;
extern bool vtg_update_flag;
extern bool heading_update_flag;
extern char GPS_buffer[GPS_BUF_NUM] ;
extern char gga_save_line[GPS_BUF_NUM];
extern char vtg_save_line[GPS_BUF_NUM];
extern char heading_save_line[GPS_BUF_NUM];
int serial_init(const char *serial_dev1,  int & fd);
bool UartReadByets(int fd,char *pbuf,int size);
bool UartGetData(char buf[], int fd);


#endif
