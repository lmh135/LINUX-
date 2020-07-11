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
#include<math.h>
#include"MySocket.h"
#include"VLP16.h"
unsigned char laser_buf[1248],recv_pos[600];
double Vertical_table[16]={-0.2618,0.0175,-0.2269,0.0524,-0.1920,0.0873,-0.1571,0.1222,-0.1222,0.1571,-0.0873,0.1920,-0.0524,0.2269,-0.0175,0.2618};
//double Vertical_table[16]={-0.2618,0.0175,-0.2269,0.0524,-0.1920,0.0873,-1.5708,0.1222,-0.1222,1.5708,-0.0873,0.1920,-0.0524,0.2269,-0.0175,0.2618};
Point point[12][16]; 



float Azimuth[12];
float distan[12][16];
unsigned long laser_timestamp = 0;
double last_timestamp;
double theta[3]={0,0,60};
double posNED[3];

bool laser_data_parse(unsigned char *buf)
{
	int i,j,k;
	unsigned long timestampbuf=(buf[1203]<<24)+(buf[1202]<<16)+(buf[1201]<<8)+buf[1200];
	double timestamp=(double)timestampbuf/1000000;//the unit is seconds
	laser_timestamp = timestampbuf;

	//printf("capture time is %.6f\n",(timestamp-last_timestamp)*1000);
	last_timestamp=timestamp;
	//printf("timestamp is %.6f\n",timestamp);
	for(i=0;i<12;i++)
	{
		Azimuth[i]=(float)((buf[3+i*100]<<8)+buf[2+i*100])/100;
		//if(Azimuth[i]>180)
			//Azimuth[i] = Azimuth[i]-360;
		//Azimuth = Azimuth[i];
	}
	
	for(i=0;i<12;i++)
	{
	    for(j=0;j<16;j++)
	    {
		distan[i][j]=(float)((buf[5+i*100+j*3]<<8)+buf[4+i*100+j*3])/1000*2;//the unit is meters
		//printf("the %d data block laser_id%d 's distance is %.3f\n",i,j,distan[i][j]);
	    }
	    //printf("\n");
	}
/*	
	for(i=0;i<12;i++)
	{
		for(j=0;j<16;j++)
		{
			point[i][j].x=distan[i][j]*cos(Vertical_table[j])*sin(Azimuth[i]*MDEG2RAD);
			point[i][j].y=distan[i][j]*cos(Vertical_table[j])*cos(Azimuth[i]*MDEG2RAD);
			point[i][j].z=distan[i][j]*sin(Vertical_table[j]);		
		}

	}
*/
	//printf("\n");
	
	return true;
}




double laser_Azimuth_parse(unsigned char *buf)
{
	double Azimuth11=0.0;
	static double last_Azimuth=0.0;
	static double last_timestamp=0.0;
	unsigned long timestampbuf=(buf[1203]<<24)+(buf[1202]<<16)+(buf[1201]<<8)+buf[1200];
	double timestamp=(double)timestampbuf/1000000;//the unit is seconds
	printf("timestamp_pkt:%.6f , diff_timestamp_pkt: %.6f \n",timestamp,(timestamp-last_timestamp));
	last_timestamp=timestamp;
	Azimuth11=(double)((buf[3]<<8)+buf[2])/100;
	printf("diff_angle_pkt: %.3f \n",(Azimuth11-last_Azimuth));
	last_Azimuth=Azimuth11;
	return Azimuth11;
}


#if 0
int main()
{
	int i,j;
	int sockfd_recv;
	Socket data_socket,position_socket;
	//printf("sin=%.3f",sin(PI/6));
	data_socket.UDPCreateServer("IPV4","UDP","192.168.1.77",2368);//lidardata
	//position_socket.UDPCreateServer("IPV4","UDP","192.168.1.77",8308);//GPS
	
	while(1)
	{	
		data_socket.UDPReceivePackets(laser_data,1248);
		//printf("data start\n");
		/*for(int i=0;i<1248;i++)		
		printf("%d ",laser_data[i]);
		printf("\n");
		printf("data end\n");
		printf("\n");
		printf("data start\n");
		for(int i=0;i<1248;i++)		
		printf("%x ",laser_data[i]);
		printf("\n");
		*/
		laser_data_parse(laser_data);
		dataHandle (point,0,posNED,theta);
		for(i=0;i<12;i++)
		for(j=0;j<16;j++)
		{
			printf("%f %f %f\n",PointXYZ[i][j].x,PointXYZ[i][j].y,PointXYZ[i][j].z);
					
		}
		//printf("data end\n");
		//printf("\n");
		
 		/*
		position_socket.UDPReceivePackets(recv_pos,554);
		printf("position start\n");
		for(int i=0;i<554;i++)		
		printf("%d ",recv_pos[i]);
		printf("\n");
		printf("position end\n");
		printf("\n");*/
	}
}
#endif
