//           g++ pthread.cpp -o pthread -lpthread 
//           ./pthread

 
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <unistd.h> 

using namespace std;
pthread_mutex_t data_mutex;

#define USEGPS 1       //使用GPS
#define USECAM 1	//使用摄像头

void *gps_capt(void*fd)
{
	ofstream gpsout;
	gpsout.open("gps.txt");
	int i=0;	
	while(1)
	{
		gpsout<<i++<<endl;
		sleep(0.1);
	}
	gpsout.close();
	return NULL;
}
void *cam_capt(void *arg)
{
	ofstream camout;
	camout.open("cam.txt");
	int i=0;	
	while(1)
	{
		camout<<i++<<endl;
		sleep(0.1);
	}
	camout.close();
	return NULL;
}

int main()
{
	int ret=0;
#if USEGPS
	int uart_fd;
	pthread_t th_uart;	
	ret=pthread_create(&th_uart,NULL,gps_capt,&uart_fd);//创建串口接收gps数据线程
	if(ret!=0)
	{
		printf("create uart thread error!\n");
		return -1;
	}
#endif

#if USECAM
	pthread_t th_cam;
	ret=pthread_create(&th_cam,NULL,cam_capt,NULL);
	if(ret!=0)
	{
		printf("create cam thread error!\n");
		return -1;
	}
#endif
	ofstream mainout;
	mainout.open("main.txt");
	int i=0;	
	while(1)
	{
		mainout<<i++<<endl;
		sleep(0.1);
	}
	mainout.close();
	pthread_join(th_uart,NULL);	
	return 0;
}
