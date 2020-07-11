//           g++ pthread.cpp -o pthread -lpthread 
//           ./pthread

 
#include <pthread.h>
#include <iostream>
#include <stdio.h>
using namespace std;
pthread_mutex_t data_mutex;

#define USEGPS 1       //使用GPS
#define USECAM 1	//使用摄像头

void *gps_capt(void*fd)
{
	int argfd = *(int*)fd;
	cout<<"gps"<<endl;
	cout<<argfd<<endl;
	return NULL;
}
void *cam_capt(void *arg)
{
	cout<<"cam"<<endl;
	return NULL;
}

int main()
{
	int ret=0;
#if USEGPS
	int uart_fd=10;
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
	cout<<"main"<<endl;
	//pthread_join(th_uart,NULL);
	//pthread_join(th_cam,NULL);
	return 0;
}
