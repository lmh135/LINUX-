#ifndef __DATAPROCESS_H
#define __DATAPROCESS_H
#include <math.h>
#include "Data.h"
#include "gps.h"
#define MDEG2RAD 0.01745329

struct Point
{
	//int laser_id;
	double offsettime;
	//double timestamp;
	//double distance;//R
	double x;	//x=R*cos(w)*sin(a)
	double y;	//y=R*cos(w)*cos(a)
	double z;	//z=R*sin(w)
	//double Azimuth;//xuan zhuan jiao a
	//double Vertical;//chui zhi jiao w
};

/*
struct Channel
{
	double x[16];	//x=R*cos(w)*sin(a)
	double y[16];	//y=R*cos(w)*cos(a)
	double z[16];	//z=R*sin(w)
	//long time_s;
	long time_us;
};
*/
//extern struct Point PointXYZ[12][16];
//extern struct Channel channel[12];
//extern Lazer_Data channel[12];

//extern Imu_Data imu_data[10];

void dataHandle (Point point[][16],unsigned int scan_idx, double posNED[], double theta[]);

//bool lazer_imu_synchronization(Packet *data);
#endif
