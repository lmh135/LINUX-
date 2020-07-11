#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>
#include<sys/time.h>
#include"MySocket.h"
#include"imu.h"
#include"uart.h"
#include"Data.h"
#include"mylib.h"
#include"gps.h"
#include"VLP16.h"



struct timeval first_time;




pthread_mutex_t time_mutex;



#define USEGPS 1       //使用GPS
#define USEIMU 1	//使用IMU
#define USECAM 1	//使用摄像头
#define USEVLP 1	//使用VLP16扫描仪
#define DEBUG 1    //是否打印调试数据



//int ggacount,vtgcount,headingcount;
//double ned_xyz_check[3],ned_uvw_check[3],heading_check;
//int gps_sats_check,gps_mod_check;
pthread_mutex_t gps_mutex;
GPS_Data gps_data_copy;

bool gga_update_flag = false;
bool vtg_update_flag = false;
bool heading_update_flag = false;


void *gps_capt(void*fd)
{
	int i;
	char GPS_buffer[GPS_BUF_NUM] ;



	
	GPS_Data tdata;
	struct timeval capt_time;
	//long long id_num = 0;

	//Socket data_gps_socket; 
	//data_gps_socket.UDPCreateClient("IPV4","UDP","192.168.1.14",2007); //地面站ip


	char gpsnewdata;
	static unsigned int gps_index = 0;
        static unsigned char new_char = 'A';
        static unsigned char last_char = 'B';
        unsigned char r = '\r' , n = '\n';
	
	 while(1)
	 {					               
		//printf("gpsgps\n");
		if (!UartReadByets(*(int*)fd, &gpsnewdata, 1))
			printf("uart recveive error\n");
		last_char = new_char;
		new_char = gpsnewdata;
		//printf("gpsaaa     %c ",new_char);
		GPS_buffer[gps_index] = new_char;
		if((last_char == r && new_char != n) ||( new_char == n && last_char != r ) )
		{  
			gps_index = 0;
			memset(GPS_buffer,'\0', GPS_BUF_NUM);
			continue;  
		}
		if(gps_index >= GPS_BUF_NUM-10)   
		{  
			gps_index = 0;
			memset(GPS_buffer,'\0', GPS_BUF_NUM);
			//printf("too many words! \n");
			continue; 
		}  
		if(last_char == r && new_char == n)//handle a line end with \r\n
		{    
			gettimeofday(&capt_time,NULL);
			//printf("get the end line! \n");                    
			GPS_buffer[gps_index] = '\0';
			if( strncmp(GPS_buffer,"$GPGGA",6)== 0)//$GPGGA
			{ 
				//printf("enter GPGGA\n");				 
				if(CharCounter(GPS_buffer,',') == 14&&(gga_update_flag == 0 ))
				{
					//printf("enter GPGGA\n");
					strncpy(gga_save_line,GPS_buffer,gps_index+1);
					memset(GPS_buffer,'\0',GPS_BUF_NUM);
					gga_update_flag=1 ;

					//pthread_mutex_lock(&gps_mutex);		     									  
					//ggacount++;
					//printf("ggacount:%d \n",ggacount);
					//pthread_mutex_unlock(&gps_mutex);		
								
				}
							
			}   
			if(strncmp(GPS_buffer,"$GPVTG",6) == 0)//$GPVTG
			{
				//printf("enter GPVTG\n");
				if(CharCounter(GPS_buffer,',') == 9&& (vtg_update_flag == 0)) //20101202   if((CharCounter(GPS_buffer,',') == 9) && (vtg_update_flag == 0)     )
				{
					strncpy(vtg_save_line,GPS_buffer,gps_index+1);
					memset(GPS_buffer,'\0',GPS_BUF_NUM);
					vtg_update_flag = 1 ;

					//pthread_mutex_lock(&gps_mutex);
					//vtgcount++;	
					//printf("vtgcount:%d \n",vtgcount);
					//pthread_mutex_unlock(&gps_mutex);
								
				}
			}
			if(strncmp(GPS_buffer,"#HEADING",8) == 0)//$GPVTG
			{
				//printf("enter heading\n");
				if(CharCounter(GPS_buffer,',') == 25&& (heading_update_flag == 0)) 
				{					
					strncpy(heading_save_line,GPS_buffer,gps_index+1);
					memset(GPS_buffer,'\0',GPS_BUF_NUM);
					heading_update_flag = 1 ;

					//pthread_mutex_lock(&gps_mutex);
					//headingcount++;
					//printf("headingcount:%d \n",headingcount);	
					//pthread_mutex_unlock(&gps_mutex);
								
				}
			}
			if(gga_update_flag || vtg_update_flag || heading_update_flag)  //if(gga_update_flag && vtg_update_flag && heading_update_flag) 
			{	
				gps_update();
				//id_num++;
				for(i=0;i<3;i++)			
				{
					tdata.ned_xyz[i] = position.current_xyz[i];
					tdata.ned_uvw[i] = position.current_uvw[i];
				}
				tdata.heading = gps_heading;
				tdata.gps_sats = gps_gga_num_sats;
				tdata.gps_mod = gps_gga_quality;

				pthread_mutex_lock(&time_mutex);
				tdata.timestamp = (capt_time.tv_sec-first_time.tv_sec)*1000000+capt_time.tv_usec-first_time.tv_usec;
				pthread_mutex_unlock(&time_mutex);

				//tdata.id_num = id_num;

				//data_gps_socket.UDPSendPackets(&tdata, sizeof(tdata));

				pthread_mutex_lock(&gps_mutex);
				gps_data_copy = tdata;
				pthread_mutex_unlock(&gps_mutex);
			
			}			
			gps_index = 0;				 
							   
			continue;
		}
		gps_index++ ;     
	}
				 
			 
			 
	
}

void *cam_capt(void *arg)
{
	int cam_count=1;
	char cmd_str[100];

	while(1)
	{
		char const *cmd = cmd_str;

		sprintf(cmd_str,"raspistill -cs 0 -t 1 -w 160 -h 120 -o %d.jpg",cam_count);

		system(cmd);
		cam_count++;

		sleep(2);
	}
}


int main()
{
	//char picname[20];
	int ret=0;
#if USEIMU
	struct timeval imu_capt_time;
	//IMU_Data imu_data;
	if(SPI_init()<0)
	{
		printf("SPI_init error\n");
		exit(1);
	}
	cout<<"SPI_init successfully!"<<endl;
#endif

#if USEGPS
	int uart_fd;
	pthread_t th_uart;
	if(serial_init("/dev/ttyAMA0",uart_fd)<0)
	{
	cout<<"ttyAMA0 port init error!"<<endl;
	exit(1);
	} 
	cout<<"open uart successfully!"<<endl;
#endif

#if USECAM
	pthread_t th_cam;
#endif

#if USEVLP
	Socket laser_capt_socket; 
	struct timeval laser_capt_time;
	//Laser_Data laser_data;

	int udprecvbuflen,udprecvbuf;
	socklen_t optlen;
	optlen=sizeof(udprecvbuf);
	laser_capt_socket.UDPCreateServer("IPV4","UDP","192.168.0.101",2368);//lidardata    //raspberry eth1-laser 192.168.0.101
 	if(getsockopt(laser_capt_socket.GetSocketFD(),SOL_SOCKET,SO_RCVBUF,(char*)&udprecvbuf,&optlen)!=0)
	perror("getsockopt1 error");
	printf("the original udprecvbuflen is %d\n",udprecvbuf);
	udprecvbuflen=0;
	if(setsockopt(laser_capt_socket.GetSocketFD(),SOL_SOCKET,SO_RCVBUF,(char*)&udprecvbuflen,sizeof(int))!=0)
	perror("setsockopt error");
	if(getsockopt(laser_capt_socket.GetSocketFD(),SOL_SOCKET,SO_RCVBUF,(char*)&udprecvbuf,&optlen)!=0)
	perror("getsockopt2 error");
	printf("the final udprecvbuflen is %d\n",udprecvbuf);
#endif


	int process_flag = 0;
	//Socket packet_send_socket; 
	Packet pkt_data;

	Packet pkt_record[50];
	int num_recv = 0;

	long long id_num = 0;
	
	//packet_send_socket.UDPCreateClient("IPV4","UDP","192.168.1.14",2006); //地面站ip

	int the_first_scan=1;
	int count_num=0;
	int i,j;

	FILE* binaryfileID=NULL;
	const char *binaryfilename="srcdata_b";
        if((binaryfileID = fopen(binaryfilename,"wb+")) == NULL )
		printf("ERROR : binary file cann't open .\n");
	

	struct timeval dwStart;
	struct timeval dwEnd;
	unsigned long dwTime;

/*
	FILE* binaryfileID=NULL;
	const char *binaryfilename="srcdata_b";
        if((binaryfileID = fopen(binaryfilename,"wb+")) == NULL )
		printf("ERROR : binary file cann't open .\n");
*/

	while(1)
	{	
		if(the_first_scan==1)
		{
			gettimeofday(&first_time,NULL);

#if USEGPS	
			ret=pthread_create(&th_uart,NULL,gps_capt,&uart_fd);//创建串口接收gps数据线程
			if(ret!=0)
			{
				printf("create uart thread error!\n");
				exit(1);
			}

#endif
#if USECAM
			ret=pthread_create(&th_cam,NULL,cam_capt,NULL);
			if(ret!=0)
			{
				printf("create cam thread error!\n");
				exit(1);
			}
#endif
			the_first_scan=0;
		}
		
		printf("*******start count_num=%d*******\n",count_num++);

		//******采集激光数据	
	
#if USEVLP	  
		fd_set fds;
		struct timeval tv;
		int r;
		while(1)
		{		  
			FD_ZERO (&fds);//将指定的?件描述符集清空
			FD_SET (laser_capt_socket.GetSocketFD(), &fds);//在文件描述符集合中增鍔????个新的文件描述符
			static int tempcount=0;
			tv.tv_sec = 0;
			tv.tv_usec = 200;
			r = select (laser_capt_socket.GetSocketFD() + 1, &fds, NULL, NULL, &tv);//判断是否可读（即摄像头是否准备好），tv是定时
			if (-1 == r)
			{
				printf ("select err\n");
			}
			if (0 == r && 1 == process_flag) 
			{
				tempcount++;
				//printf("tempcount=%d\n",tempcount);
				if(tempcount>30)
				{
//******数据记录开始*************
					gettimeofday(&dwStart,NULL);
					tempcount=0;
					if(count_num>10)
						for(i=1;i<num_recv;i++)
						{
							fwrite(&(pkt_record[i]),sizeof(Packet),1,binaryfileID);
						}
					num_recv = 0;
					memset(pkt_record, 0, sizeof(pkt_record));
					process_flag = 0;

					gettimeofday(&dwEnd,NULL);
					dwTime=1000000*(dwEnd.tv_sec-dwStart.tv_sec)+(dwEnd.tv_usec-dwStart.tv_usec);

#if DEBUG
					printf("data record process time=%ldus\n",dwTime);
#endif
//******数据记录开始*************
				}
				else
					continue;
					
			}
			else
				break;
		}

		memset(laser_buf, 0, sizeof(laser_buf));

		gettimeofday(&dwStart,NULL);
		laser_capt_socket.UDPReceivePackets(laser_buf,1248);
		gettimeofday(&laser_capt_time,NULL);

		dwTime=1000000*(laser_capt_time.tv_sec-dwStart.tv_sec)+(laser_capt_time.tv_usec-dwStart.tv_usec);
#if DEBUG
		printf("laser data capture time=%ldus\n",dwTime);
#endif
		
#endif		
		//******激光数据采集结束

		//******IMU数据采集开始
		gettimeofday(&dwStart,NULL);
		take_SPI_data(spi_fd);
		gettimeofday(&imu_capt_time,NULL);

		dwTime=1000000*(imu_capt_time.tv_sec-dwStart.tv_sec)+(imu_capt_time.tv_usec-dwStart.tv_usec);
#if DEBUG
		printf("imu data capture time=%ldus\n",dwTime);
#endif
		//******IMU数据采集结束

		//******数据整合
		id_num++;

		laser_data_parse(laser_buf);
#if DEBUG
		laser_Azimuth_parse(laser_buf);	
#endif
		//memset(&laser_data, 0, sizeof(laser_data));
		for(i=0;i<12;i++)
		{
			pkt_data.laser_data.Azimuth[i] = Azimuth[i];
			for(j=0;j<16;j++)
				pkt_data.laser_data.distance[i][j] = distan[i][j];
		}

		pkt_data.imu_data.imu_gyro[0] = imu_gyro[0];
		pkt_data.imu_data.imu_gyro[1] = -imu_gyro[1];
		pkt_data.imu_data.imu_gyro[2] = -imu_gyro[2];
		pkt_data.imu_data.imu_accel[0] = imu_accel[0];
		pkt_data.imu_data.imu_accel[1] = -imu_accel[1];
		pkt_data.imu_data.imu_accel[2] = -imu_accel[2];
		pkt_data.imu_data.imu_magn[0] = imu_magn[0];
		pkt_data.imu_data.imu_magn[1] = -imu_magn[1];
		pkt_data.imu_data.imu_magn[2] = -imu_magn[2];
		pkt_data.imu_data.imu_euler[0] = imu_angle[0];
		pkt_data.imu_data.imu_euler[1] = -imu_angle[1];
		pkt_data.imu_data.imu_euler[2] = -imu_angle[2];

		pthread_mutex_lock(&time_mutex);
		pkt_data.imu_data.timestamp = (imu_capt_time.tv_sec-first_time.tv_sec)*1000000+imu_capt_time.tv_usec-first_time.tv_usec;
		pkt_data.laser_data.timestamp = (laser_capt_time.tv_sec-first_time.tv_sec)*1000000+laser_capt_time.tv_usec-first_time.tv_usec;
		pthread_mutex_unlock(&time_mutex);

		pthread_mutex_lock(&gps_mutex);
		pkt_data.gps_data = gps_data_copy;
		pthread_mutex_unlock(&gps_mutex);

		pkt_data.id_num = id_num;

		//******整合结束

		//*********数据监控*********
#if DEBUG
		//printf("*******imudata*******\n");

		printf("roll:%lf   pitch:%lf   yaw:%lf \n",pkt_data.imu_data.imu_euler[0],pkt_data.imu_data.imu_euler[1],pkt_data.imu_data.imu_euler[2]);	

		//printf("*******imudata end*******\n");


		//printf("*******gpsdata*******\n");

		//pthread_mutex_lock(&gps_mutex);
		printf("ned_xyz: %.3f %.3f %.3f \n",pkt_data.gps_data.ned_xyz[0],pkt_data.gps_data.ned_xyz[1],pkt_data.gps_data.ned_xyz[2]);
		printf("ned_uvw: %.3f %.3f %.3f \n",pkt_data.gps_data.ned_uvw[0],pkt_data.gps_data.ned_uvw[1],pkt_data.gps_data.ned_uvw[2]);
		printf("heading:%.3f \n",pkt_data.gps_data.heading);
		printf("sats_num:%d   gps_mod:%d \n",pkt_data.gps_data.gps_sats,pkt_data.gps_data.gps_mod);			
		//pthread_mutex_unlock(&gps_mutex);

		//printf("*******gpsdata end*******\n");
#endif
		pkt_record[num_recv] = pkt_data;
		num_recv++;	
		process_flag = 1;








				
#if DEBUG
		printf("*******end*******\n");
		printf("\n");
#endif
			
			
		
	}
}





		
