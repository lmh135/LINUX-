#ifndef __VLP16_
#define __VLP16_
#include"dataprocess.h"

#define T_DELAY_PACK	0//打包时延 us
#define T_DELAY_TRANS	0//发送时延 us
#define T_DELAY_SYS	0//系统时延 us
#define AnglePerUs	0.0036//每微秒转的度数 现600n/min

extern unsigned char laser_buf[1248];
extern Point point[12][16]; 

extern float Azimuth[12];
extern float distan[12][16];
extern unsigned long laser_timestamp;


bool laser_data_parse(unsigned char *buf);
//bool laser_data_parse(unsigned char *buf, unsigned long time_us);
double laser_Azimuth_parse(unsigned char *buf);

#endif
