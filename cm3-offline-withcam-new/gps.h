#ifndef _GPS_H_
#define _GPS_H_
#include"uart.h"
 //#define GPS_FREQ 5;
#define GPS_FREQ 20;

struct Position
{
	//void      reset();
	//void      current_target_update(double next_target[3]);
                            
	double current_xyz[3];   //·É»úÎ»ÖÃ¡¢ËÙ¶È£šNED£©ºÍºœÏò
	double current_uvw[3];
                                
	double current_target[3];   //·ÉÐÐ×ŽÌ¬ºÍµ±Ç°Ä¿±êµã
	double heading ;            //µ±Ç°žø¶šÄ¿±êœÇ¶È
	
};
extern struct Position position;
extern double input_xyz[3];
extern double input_yaw;
extern double input_uvw[3];
extern double current_vel[3];

extern  int    gps_update_flag;

extern	double gps_gga_time;
extern	double gps_gga_latitude;
extern	double gps_gga_longitude;
extern	int gps_gga_quality;
extern	int gps_gga_num_sats;
extern	double gps_gga_hdop;
extern	double gps_gga_altitude;
extern	double gps_gga_wgs_alt;

extern double gps_heading;
extern double gps_pitch;


extern	double gps_vtg_course;
extern	double gps_vtg_speed;

extern char gga_save_line[GPS_BUF_NUM];
extern char vtg_save_line[GPS_BUF_NUM];
extern char heading_save_line[GPS_BUF_NUM];
        

extern int strntol(char *			line,size_t			len,char **			line_out,int			base);


extern void ned2bf2(const double v_in[3],const double theta, double v_out[3]) ;
extern void gps_date_init(void);
extern void gpgga_update(void);
extern void gpvtg_update(void);
extern void heading_update(void);
extern double yaw_convert(const double yaw, const double t_head);
extern void gps2xyz(double *llh);
extern void gps2uvw(double * vel_NED);
extern void gps_monitor(void);
extern void gps_update(void);
extern double zero_position0 ,zero_position1,zero_position2;
#endif

