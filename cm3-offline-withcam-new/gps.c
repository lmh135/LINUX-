#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include"gps.h"
#include"conversion.h"
#define FILTER_LEN 3
#define PI 3.14159265
struct Position position;
int gps_update_flag ;
double input_xyz[3];
double input_yaw=0;
double input_uvw[3]={0.1,0.2,0.3};
double current_vel[3]; 

/*
double zero_position0= 23.0419933*C_DEG2RAD,//23.1565516*C_DEG2RAD  //0.404671487796//
       zero_position1= 113.40246*C_DEG2RAD,//113.341556*C_DEG2RAD
       zero_position2= 38.0;
*/
//wws 20110530: set the zero_position in tiantai, biaoshi de difang.//
/*
double zero_position0= 23.1565695*C_DEG2RAD,
       zero_position1= 113.3415682*C_DEG2RAD,
       zero_position2= 47.576;//zero_position2 = gps_gga_altitude+gps_gga_wgs_alt;
*/
//wws 20110725: set the zero_position in kexuecheng.//
//double zero_position0= 23.1678814*C_DEG2RAD,
       //zero_position1= 113.3981862*C_DEG2RAD,
       //zero_position2= 27.964;//zero_position2 = gps_gga_altitude+gps_gga_wgs_alt;
double zero_position0,zero_position1,zero_position2;
double zero_position[3];
double gps_gga_time;
double gps_gga_latitude;
double gps_gga_longitude;
int gps_gga_quality;
int gps_gga_num_sats;
int last_gps_gga_num_sats;
double gps_gga_hdop;
double gps_gga_altitude;
double gps_gga_wgs_alt;

double gps_heading;
double gps_pitch;


char gga_save_line[GPS_BUF_NUM];
char vtg_save_line[GPS_BUF_NUM];
char heading_save_line[GPS_BUF_NUM];

double gps_vtg_course;
double gps_vtg_speed;
double f,e,zero_position_ECEF[3],zero[3];
double postion_ECEF[3];
double Re2t[3][3];

/*****************wws 20100930********************/
extern double theta[3];
/*************************************/
inline void ECEF2Tangent(double *ECFF);
/*---------------------------------------------------------------------------------*/     
inline  double sqr(double	x)
{
	return (x * x);
}      

void ned2bf2(const double v_in[3],const double theta,double v_out[3])
{
  static double     cos_theta   =   1.0;
  static  double    sin_theta   =   0.0;
  static  double    last_theta  =   0.0;
  if( last_theta != theta)
  {
      last_theta   =   theta;
      cos_theta    =   cos(theta) ;
      sin_theta    =   sin(theta) ;
  }
  
  v_out[0]  = v_in[0] * cos_theta + v_in[1] * sin_theta ;
  v_out[1]  = v_in[1] * cos_theta - v_in[0] * sin_theta ;     
  v_out[2]  = v_in[2] ;
}


/*---------------------------------------------------------------------------------*/
int strntol(char *line,size_t	len,char **line_out,int base)
{
	char			buf[ 8 ];
	if( len > 8 )
		len = 8;

	strncpy( buf, line, len );

	buf[len] = '\0';

	if( line_out )
		*line_out = line + len;

	return strtol( buf, 0, base );
}
/*---------------------------------------------------------------------------------*/
/********************************************************
* º¯ÊýËµÃ÷£º * gpsÊýŸÝ³õÊŒ»¯
               È¡µÚ10×éÓÐÐ§µÄGPSÐÅÏ¢£¬³õÊŒ»¯NEDÔ­µãµÄŸ­Î³¶ÈºÍº£°Î
               ÀûÓÃÕâžö³õÊŒNEDÔ­µã£¬»ñµÃECEF×ª»»µœNEDµÄ×ø±ê×ª»»ŸØÕóRe2t
* Èë¿Ú²ÎÊý£º * ÎÞ 
* ·µ»Ø²ÎÊý£º * ÎÞ
* ÊäÈëÊýŸÝÊŸÀý£º  
* ×îºóÐÞžÄ£º * ³ÂÓÂ£¬2009-03-26
* ±ž    ×¢£º * ÎÞ
********************************************************/ 
void gps_date_init(void)
{
   static unsigned int first_entry = 1 ;
    //if( first_entry == 100 )
    if( first_entry == 1 )
    {
      // wws 20110530: we have set the zero_position012//   
         zero_position0 = gps_gga_latitude*C_DEG2RAD;
         zero_position1 = gps_gga_longitude*C_DEG2RAD;
         zero_position2 = gps_gga_altitude+gps_gga_wgs_alt;
         zero_position[0]=zero_position0;
         zero_position[1]=zero_position1;
         zero_position[2]=zero_position2;
         f = ((C_WGS84_a - C_WGS84_b) / C_WGS84_a);
         e = sqrt( 2*f - f*f );
         double N = C_WGS84_a/sqrt( 1 - e*e*sqr(sin(zero_position0)));
         zero_position_ECEF[0]=(N + zero_position2) * cos(zero_position0) * cos(zero_position1);
         zero_position_ECEF[1]=(N + zero_position2) * cos(zero_position0) * sin(zero_position1);
         zero_position_ECEF[2]=(N*(1-e*e) + zero_position2) * sin(zero_position0);
         zero[0]=zero_position_ECEF[0];
         zero[1]=zero_position_ECEF[1];
         zero[2]=zero_position_ECEF[2];
         
         double		clat = cos(zero_position0);
         double		clon = cos(zero_position1);
         double		slat = sin(zero_position0);
         double		slon = sin(zero_position1);

	Re2t[0][0] = -slat*clon;
	Re2t[0][1] = -slat*slon;
	Re2t[0][2] =  clat;

	Re2t[1][0] = -slon;
	Re2t[1][1] =  clon;
	Re2t[1][2] =  0.0;

	Re2t[2][0] = -clat*clon;
	Re2t[2][1] = -clat*slon;
	Re2t[2][2] = -slat;
        ECEF2Tangent(zero);       //地心坐标系转成北东地坐标系，x轴指北，y轴指东，z轴指地
        
   }
   
    first_entry ++ ;
    if(first_entry >=700)
      first_entry = 650;
}

/*---------------------------------------------------------------------------------*/     
inline void llh2ECEF( double *llh)
{
	double value[3];
	double	N = C_WGS84_a/sqrt( 1 - e*e*sqr(sin(llh[0])));

	value[0]= (N + llh[2]) * cos(llh[0]) * cos(llh[1]);
        value[1]= (N + llh[2]) * cos(llh[0]) * sin(llh[1]);
	value[2]= (N*(1-e*e) + llh[2]) * sin(llh[0]);
        llh[0] = value[0];
        llh[1] = value[1];
        llh[2] = value[2];
	return ;
}

inline void ECEF2Tangent(double *ECFF)
{
	double value[3];
        
        value[0] = Re2t[0][0]*ECFF[0]+Re2t[0][1]*ECFF[1]+Re2t[0][2]*ECFF[2];
        value[1] = Re2t[1][0]*ECFF[0]+Re2t[1][1]*ECFF[1]+Re2t[1][2]*ECFF[2];
        value[2] = Re2t[2][0]*ECFF[0]+Re2t[2][1]*ECFF[1]+Re2t[2][2]*ECFF[2];
        
        ECFF[0] = value[0];
        ECFF[1] = value[1];
        ECFF[2] = value[2];
        //value[0]=(Re2t*ECEF)[0];value[1]=(Re2t*ECEF)[1];value[2]=(Re2t*ECEF)[2];
	return ;
}          
          
/*---------------------------------------------------------------------------------*/

//--------------------------------------------------------------
/********************************************************
* º¯ÊýËµÃ÷£º * œ«GPSÊýŸÝÖÐµÄGGA²¿·ÖœâÎö³öÀŽ
               È«ŸÖ±äÁ¿gps_gga_time£¬gps_gga_latitude£¬gps_gga_longitude
               gps_gga_quality£¬gps_gga_num_sats£¬gps_gga_hdop£¬ gps_gga_altitude
               gps_gga_wgs_alt žüÐÂ
* Èë¿Ú²ÎÊý£º * ÎÞ 
* ·µ»Ø²ÎÊý£º * ÎÞ
* ÊäÈëÊýŸÝÊŸÀý£º * $GPGGA,102134.00,1306.9847,N,11402.2986,W,2,5,1.0,50.3,M,-16.27,M,,*61\r\n  
* ×îºóÐÞžÄ£º * ³ÂÓÂ£¬2009-03-26
* ±ž    ×¢£º * ÎÞ
********************************************************/ 
void gpgga_update(void)
{
         int lat = 0 ;
         double lat_min = 0.0;
        
         char * line ;
         line = index(gga_save_line,',');
         
         line++; 
        gps_gga_time =  strtod(line,&line);
        int hour = (int)(gps_gga_time/10000.0);
        int min = (int)((gps_gga_time-hour*10000)/100.0);
        float sec   = gps_gga_time-hour*10000-min*100;
         gps_gga_time =  (min * 60 + sec)*100;  //µ¥Î»ÊÇ0.01m,ŽÓ×îºóÁœÎ»¿ÉÒÔÖªµÀGPSœÓÊÕÖÜÆÚ
        
        //------Î³¶È---- 
        line++;
        gps_gga_latitude     = strtod( line, &line);
        lat = (int)(gps_gga_latitude/100.0);
        lat_min = gps_gga_latitude - lat*100.0;
        gps_gga_latitude = lat + lat_min / 60.0;
        
        
	line++;
	if( *line == 'S' )      // Find the sign for the lattitude.  South is negative
	  gps_gga_latitude *= -1;
	line = index(line,',');               // Skip ,S,
        
        //------Ÿ­¶È---- 
        line++;
        gps_gga_longitude = strtod( line, &line);
	lat= (int)(gps_gga_longitude/100.0);
	lat_min= gps_gga_longitude - lat*100 ;
	gps_gga_longitude = lat + lat_min / 60.0;
	
        line++;
	if( *line == 'W' )
	   gps_gga_longitude *= -1;
	line = index(line,',');
	
        //----²î·Ö×ŽÌ¬
        line++;
	gps_gga_quality = strtol( line, &line, 10 );
	line++;		// Skip ,
        //-------ÐÇÊý
	gps_gga_num_sats = strtol( line, &line, 10 );
	line++;		// Skip ,
        
	gps_gga_hdop = strtod( line, &line );
	line++;		// Skip ,

	gps_gga_altitude = strtod( line, &line );  //º£°Î
	line += 3;	// Skip ,M,
        
       // line++;
	gps_gga_wgs_alt = strtod( line, &line );  //ŽóµØË®ÆœÃæÓëµØÇòÍÖÔ²Ãæžß¶È²î
	line += 3;	// Skip ,M,
        return;
}

/*---------------------------------------------------------------------------------*/     
          
          
/*---------------------------------------------------------------------------------*/
void gpvtg_update(void)
{
  char * line=NULL ; 
  line = index(vtg_save_line,',');
  if(!line)
      return ;
  line++;//skip ,
  gps_vtg_course = strtod( line, &line );
  line = index(line,'N');
  if(!line)
      return;
  line += 2;
  gps_vtg_speed = strtod( line, &line ) * 1000.0 / 3600.0;
  return;
}

/*---------------------------------------------------------------------------------*/     
          
          
/*---------------------------------------------------------------------------------*/
void heading_update(void)
{
         char * line ;
         line = index(heading_save_line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 
         line = index(line,',');
         line++; 

	 gps_heading = strtod(line,&line);
	 if(gps_heading>180.0f)
		gps_heading = gps_heading-360.0f;
	 line++; 

	 gps_pitch = strtod(line,&line);

         return;
}





          
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
void gps2xyz(double *llh)
{

  llh[0] = gps_gga_latitude*C_DEG2RAD;
  llh[1] = gps_gga_longitude*C_DEG2RAD;
  llh[2] = gps_gga_altitude+gps_gga_wgs_alt;
  llh2ECEF( llh );
  postion_ECEF[0]=llh[0];
  postion_ECEF[1]=llh[1];
  postion_ECEF[2]=llh[2];
  
//  llh[0] = llh[0] - zero_position_ECEF[0];
//  llh[1] = llh[1] - zero_position_ECEF[1];
//  llh[2] = llh[2] - zero_position_ECEF[2];
 //llh[0] = llh[0] - (-2297207.236669);
 //llh[1] = llh[1] - (5378841.070419);
 //llh[2] = llh[2] - (2535582.567793);

  // llh[0] = llh[0] - (-2278068.184508);//sanshui
  // llh[1] = llh[1] - (5394530.269425);
  // llh[2] = llh[2] - (2519387.817044);

//  llh[0] = llh[0]-(-2297043.502152) ;//tiantai
//  llh[1] = llh[1]-(5378862.041528);
//  llh[2] = llh[2]-(2535680.409950);
  llh[0] = llh[0]-(-2324699.253257) ;//tiantai
  llh[1] = llh[1]-(5387162.520920);
  llh[2] = llh[2]-(2492690.225375);

  //llh[0] = llh[0]-(-2337131.255968) ;//luogang
  //llh[1] = llh[1]-(5381264.142896);
  //llh[2] = llh[2]-(2493745.220211);
  //llh[0] = llh[0]-(-2340450.677617) ;//jiulong
  //llh[1] = llh[1]-(5371788.101780);
  //llh[2] = llh[2]-(2510939.430029);

  ECEF2Tangent(llh);
}

/*---------------------------------------------------------------------------------*/     

 void gps2uvw(double *vel_NED)
{   
    int i;
    static double last_Z = 0.0;               
    double vel_z_current =0.0 ;
    //GPS
    vel_NED[0] = gps_vtg_speed * cos( gps_vtg_course * C_DEG2RAD );
    vel_NED[1]  = gps_vtg_speed * sin( gps_vtg_course * C_DEG2RAD );

    //vel_NED[0] = gps_vtg_speed * cos( cps_theta[2] * C_DEG2RAD );
    //vel_NED[1]  = gps_vtg_speed * sin( cps_theta[2] * C_DEG2RAD );
    
    if(last_gps_gga_num_sats != gps_gga_num_sats)
    {
       last_gps_gga_num_sats = gps_gga_num_sats;
       last_Z = position.current_xyz[2];
       return ;
    }
    vel_z_current = (position.current_xyz[2] - last_Z)*GPS_FREQ ;
    
    float sum_vel_z =0.0; 
    static float window_vel_z[FILTER_LEN] = {0.0,0.0} ;  // zÏòËÙ¶ÈŒÓ2µÄÆœŸùÖµÂË²š
    
    for( i =FILTER_LEN-1 ; i != 0 ; --i )
    {
        window_vel_z[i] = window_vel_z[i-1];
        sum_vel_z += window_vel_z[i];
     }
    
     window_vel_z[0] = vel_z_current ;
     sum_vel_z += window_vel_z[0] ;
      vel_NED[2] = sum_vel_z/FILTER_LEN ;        
 
    last_Z = position.current_xyz[2];
     
    return ;   
  
}
/*---------------------------------------------------------------------------------*/   
/*
void xyz_transform(double input_xyz[3], Position * position_t,Trajectory * trajectory_t){
  
    	input_xyz[0] = (position_t->current_xyz[0]-position_t->current_target[0])*cos(position_t->heading)
	           +(position_t->current_xyz[1]-position_t->current_target[1])*sin(position_t->heading); 
			   
	input_xyz[1] = (position_t->current_xyz[1]-position_t->current_target[1])*cos(position_t->heading)
			   -(position_t->current_xyz[0]-position_t->current_target[0])*sin(position_t->heading);    

	input_xyz[2] =  (position_t->current_xyz[2]-position_t->current_target[2]);
  
}      
*/  
/******************perfect transform maxtri for use,use hrm3000 cps_theta[2] as the real yaw **************/
/*   ////wrong:position_t->current_xyz[1]*cos(theta[0])*sin(theta[1])*sin(cps_yaw*C_DEG2RAD)
void xyz_transform(double input_xyz[3], Position * position_t,const double cps_yaw){
  
    	input_xyz[0] = position_t->current_xyz[0]*cos(cps_yaw*C_DEG2RAD)*cos(theta[1])
	               + position_t->current_xyz[1]*sin(cps_yaw*C_DEG2RAD)*cos(theta[1])
                       - position_t->current_xyz[2]*sin(theta[1]); 
			   
	input_xyz[1] = position_t->current_xyz[0]*cos(cps_yaw*C_DEG2RAD)*sin(theta[0])*sin(theta[1])
                       - position_t->current_xyz[0]*sin(cps_yaw*C_DEG2RAD)*cos(theta[0])
                       + position_t->current_xyz[1]*cos(cps_yaw*C_DEG2RAD)*cos(theta[0])
                       + position_t->current_xyz[1]*cos(theta[0])*sin(theta[1])*sin(cps_yaw*C_DEG2RAD)
                       + position_t->current_xyz[2]*cos(theta[1])*sin(theta[0]);    

	input_xyz[2] = position_t->current_xyz[0]*sin(theta[0])*sin(cps_yaw*C_DEG2RAD)
                       + position_t->current_xyz[0]*cos(cps_yaw*C_DEG2RAD)*sin(theta[1])*cos(theta[0])
                       - position_t->current_xyz[1]*cos(cps_yaw*C_DEG2RAD)*sin(theta[0])
                       + position_t->current_xyz[1]*sin(cps_yaw*C_DEG2RAD)*sin(theta[1])*cos(theta[0])
                       + position_t->current_xyz[2]*cos(theta[0])*cos(theta[1]);
  
}  
*/


/*
void xyz_transform(double input_xyz[3], struct Position * position_t,const double cps_yaw){
  
    	input_xyz[0] = position_t->current_xyz[0]*cos(cps_yaw*C_DEG2RAD)*cos(theta[1])
	               + position_t->current_xyz[1]*sin(cps_yaw*C_DEG2RAD)*cos(theta[1])
                       - position_t->current_xyz[2]*sin(theta[1]); 
			   
	input_xyz[1] = position_t->current_xyz[0]*cos(cps_yaw*C_DEG2RAD)*sin(theta[0])*sin(theta[1])
                       - position_t->current_xyz[0]*sin(cps_yaw*C_DEG2RAD)*cos(theta[0])
                       + position_t->current_xyz[1]*cos(cps_yaw*C_DEG2RAD)*cos(theta[0])
                       + position_t->current_xyz[1]*sin(theta[0])*sin(theta[1])*sin(cps_yaw*C_DEG2RAD)
                       + position_t->current_xyz[2]*cos(theta[1])*sin(theta[0]);    

	input_xyz[2] = position_t->current_xyz[0]*sin(theta[0])*sin(cps_yaw*C_DEG2RAD)
                       + position_t->current_xyz[0]*cos(cps_yaw*C_DEG2RAD)*sin(theta[1])*cos(theta[0])
                       - position_t->current_xyz[1]*cos(cps_yaw*C_DEG2RAD)*sin(theta[0])
                       + position_t->current_xyz[1]*sin(cps_yaw*C_DEG2RAD)*sin(theta[1])*cos(theta[0])
                       + position_t->current_xyz[2]*cos(theta[0])*cos(theta[1]);  
}


*/
/*---------------------------------------------------------------------------------*/
//void uvw_transform(double *vel_bf, Position * position_t,const double cps_yaw)
// erro:position_t->current_uvw[1]*cos(theta[0])*sin(theta[1])*sin(cps_yaw*C_DEG2RAD)

/*

void uvw_transform(double input_uvw[3], struct Position * position_t,const double cps_yaw)
{
    input_uvw[0] = position_t->current_uvw[0]*cos(cps_yaw*C_DEG2RAD)*cos(theta[1])
	        + position_t->current_uvw[1]*sin(cps_yaw*C_DEG2RAD)*cos(theta[1])
                - position_t->current_uvw[2]*sin(theta[1]); 
    
    input_uvw[1] = position_t->current_uvw[0]*cos(cps_yaw*C_DEG2RAD)*sin(theta[0])*sin(theta[1])
                - position_t->current_uvw[0]*sin(cps_yaw*C_DEG2RAD)*cos(theta[0])
                + position_t->current_uvw[1]*cos(cps_yaw*C_DEG2RAD)*cos(theta[0])
                + position_t->current_uvw[1]*sin(theta[0])*sin(theta[1])*sin(cps_yaw*C_DEG2RAD)
                + position_t->current_uvw[2]*cos(theta[1])*sin(theta[0]); 
    
    input_uvw[2] = position_t->current_uvw[0]*sin(theta[0])*sin(cps_yaw*C_DEG2RAD)
                + position_t->current_uvw[0]*cos(cps_yaw*C_DEG2RAD)*sin(theta[1])*cos(theta[0])
                - position_t->current_uvw[1]*cos(cps_yaw*C_DEG2RAD)*sin(theta[0])
                + position_t->current_uvw[1]*sin(cps_yaw*C_DEG2RAD)*sin(theta[1])*cos(theta[0])
                + position_t->current_uvw[2]*cos(theta[0])*cos(theta[1]);    
}
*/
/*---------------------------------------------------------------------------------*/     
          
          
/*---------------------------------------------------------------------------------*/

double yaw_convert(const double yaw, const double t_head)
{
   if(t_head > PI/2)
       if(yaw < -PI/2)
       return yaw+2*PI;
   if(t_head < -PI/2)
       if(yaw >  PI/2)
       return yaw-2*PI;
   return yaw; 
} 
/*---------------------------------------------------------------------------------*/     
          
          


/*---------------------------------------------------------------------------------*/     
          
          


void gps_monitor()
{
    static double last_vtg_course ,
                  last_vtg_speed  ,
                  last_gga_time   ;
    
    static int speed_flag = 0,
               course_flag = 0,
               time_flag = 0;
        
    if(last_vtg_course == gps_vtg_course )
       course_flag ++;
    else 
       course_flag = 0 ;
    last_vtg_course = gps_vtg_course;
    
    if(last_vtg_speed == gps_vtg_speed )
       speed_flag ++;
    else 
       speed_flag = 0 ;
    last_vtg_speed = gps_vtg_speed ;
    
    
    if(last_gga_time == gps_gga_time)
       time_flag ++;
    else
       time_flag =0 ;
    last_gga_time = gps_gga_time ;
    
    if(course_flag >=30 && speed_flag >= 30 && time_flag >=30 )  //30žö¿ØÖÆÖÜÆÚÄÚÃ»ÓÐžüÐÂgpsÖµ
    {   
        gps_update_flag = 2;        //gpsÐÅºÅÖÐ¶Ï
      /*  trajectory.reset();         // ********ŽŠÀígpsÖÐ¶ÏµÄÇé¿ö************
        position.current_uvw[0]= 0;
        position.current_uvw[1]= 0;
        position.current_uvw[2]= 0;
        trajectory.heli_step = 1;   //ÊÖ¿Ø×ŽÌ¬ÎÞÓ°Ïì*/
    }   
}

void gps_update()
{
     bool gps_sample = false;
     if(gga_update_flag==true ) // œÓÊÕµœÒ»Ž®ÊýŸÝ
     {
         gpgga_update();
         gga_update_flag=false;   
         gps_sample = true;
     }
    
     if(vtg_update_flag==true)// && (gps_gga_quality!=0))   // in interrupt_usart.c
     {
        gpvtg_update();
        vtg_update_flag = false;
        gps_sample = true;
     } 

     if(heading_update_flag==true)// && (gps_gga_quality!=0))   // in interrupt_usart.c
     {
        heading_update();
        heading_update_flag = false;
        gps_sample = true;
     } 
     
     if(gps_sample==true)
     {
         gps_sample = false;

        // if((gps_gga_quality==1 ||gps_gga_quality==2||gps_gga_quality==4 || gps_gga_quality ==5 ) &&
         //   (gps_gga_num_sats>=4))

         if((gps_gga_quality>=4) && (gps_gga_num_sats>=4))
         {
             gps_update_flag = 1;
             gps_date_init();
             gps2xyz(position.current_xyz);    //update current_xyz£¬Ë³Ðò²»ÄÜ·Ž
             gps2uvw(position.current_uvw);    //update speed
            // printf("zeroecef0=%f   zeroecef1=%f   zeroecef2=%f\n",zero_position_ECEF[0],zero_position_ECEF[1],zero_position_ECEF[2]);
	    // printf("zero0=%f   zero1=%f   zero2=%f\n",zero[0],zero[1],zero[2]);
	    // printf("gps_heading:%lf \n",gps_heading);
         }
         else //set to abnormal mod, all to zero.
         {
              gps_update_flag = 2;
           /********   
              trajectory.reset();     // ********ŽŠÀígpsÖÐ¶ÏµÄÇé¿ö************
              position.current_uvw[0]= 0;
              position.current_uvw[1]= 0;
              position.current_uvw[2]= 0;
              position.current_target[0] = position.current_xyz[0] ;
              position.current_target[1] =  position.current_xyz[1];
              position.current_target[2] =  position.current_xyz[2];
              
              trajectory.heli_step = 1;   //ÊÖ¿Ø×ŽÌ¬ÎÞÓ°Ïì  
          *********/
         }
     }//end if(gps_sample = 1)  
  
}
 
