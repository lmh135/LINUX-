#ifndef _IMU_H_
#define _IMU_H_

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <math.h>

#define ADIS16480 1
#define ADIS16365 0
/************************/
/*for the new main board*/
/************************/
#define C_DEG2RAD 0.01745329251994

#if ADIS16365 
#define INDEX_CX 0//INDEX_P 1//
#define INDEX_CY 1//INDEX_Q 6//
#define INDEX_CZ 2//INDEX_R 8//
                                                                                                 
#define INDEX_AX 4//INDEX_AX 0//
#define INDEX_AY 5//INDEX_AY 5//
#define INDEX_AZ 6//INDEX_AZ 7// 
                                                                                                 
#define INDEX_GX 1//INDEX_CX 3//
#define INDEX_GY 2//INDEX_CY 2//
#define INDEX_GZ 3//INDEX_CZ 4//

#define MAX_ADC_SAMPLES 9

#define bias_ax 32768  //23124//22235 //18621//22235 //31605               //
#define bias_ay 32768  //18779//22913 //22937 //39296
#define bias_az 32768  //24731//17439 //24634  //37797
    
/******wws: ADIS16360/65 is 3.333mg , ADIS16350/55 is 2.522mg*****/
#define accel_scale_x  0.03333*0.98//0.02522*0.98//-0.00116046//-0.00112843 //0.00123505        //   Unit:  G/bit
#define accel_scale_y  -0.03333*0.98//-0.02522*0.98//0.00123645//0.00123482 //-0.00115821  
#define accel_scale_z  -0.03333*0.98//-0.02522*0.98//0.00117513//0.00118435 //-0.00118300  
/******wws: we now use the range of 75du/sec,so our accuracy is 0.0125 **************/                                                                                                 
#define gyro_scale_x  0.05*C_DEG2RAD   //0.0125*C_DEG2RAD   //0.07526*1.5*C_DEG2RAD  
#define gyro_scale_y  -0.05*C_DEG2RAD  //-0.0125*C_DEG2RAD  //-0.07526*1.5*C_DEG2RAD
#define gyro_scale_z  -0.05*C_DEG2RAD  //-0.0125*C_DEG2RAD   //-0.07526*1.2*C_DEG2RAD
                            
#endif

#if ADIS16480
#define INDEX_CX 0//INDEX_P 1//
#define INDEX_CY 1//INDEX_Q 6//
#define INDEX_CZ 2//INDEX_R 8//
                                
#define INDEX_MX 15//INDEX_AX 0//
#define INDEX_MY 17//INDEX_AY 5//
#define INDEX_MZ 19//INDEX_AZ 7// 

#define INDEX_AX 9//INDEX_AX 0//
#define INDEX_AY 11//INDEX_AY 5//
#define INDEX_AZ 13//INDEX_AZ 7// 
                                                                                                 
#define INDEX_GX 3//INDEX_CX 3//
#define INDEX_GY 5//INDEX_CY 2//
#define INDEX_GZ 7//INDEX_CZ 4//

#define INDEX_ROLL  21//INDEX_P 1//
#define INDEX_PITCH 23//INDEX_Q 6//
#define INDEX_YAW   25//INDEX_R 8//

#define MAX_ADC_SAMPLES 9

#define bias_ax 32768  //23124//22235 //18621//22235 //31605               //
#define bias_ay 32768  //18779//22913 //22937 //39296
#define bias_az 32768  //24731//17439 //24634  //37797
    
/******wws:  ADIS16480 is 0.8mg,ADIS16360/65 is 3.333mg , ADIS16350/55 is 2.522mg*****/
#define accel_scale_x   0.0008*9.8//0.02522*0.98//-0.00116046//-0.00112843 //0.00123505        //   Unit:  G/bit
#define accel_scale_y  0.0008*9.8//-0.02522*0.98//0.00123645//0.00123482 //-0.00115821  
#define accel_scale_z  0.0008*9.8//-0.02522*0.98//0.00117513//0.00118435 //-0.00118300  
/******wws: we now use the range of 75du/sec,so our accuracy is 0.0125 **************/                                                                                                 
#define gyro_scale_x  0.02*C_DEG2RAD   //0.0125*C_DEG2RAD   //0.07526*1.5*C_DEG2RAD     
#define gyro_scale_y 0.02*C_DEG2RAD  //-0.0125*C_DEG2RAD  //-0.07526*1.5*C_DEG2RAD    
#define gyro_scale_z 0.02*C_DEG2RAD  //-0.0125*C_DEG2RAD   //-0.07526*1.2*C_DEG2RAD   

#define magn_scale_x   0.1   //mgus 0.1
#define magn_scale_y  0.1  //mgus
#define magn_scale_z  0.1  //mgus

#define angle_scale_x     0.0054931641  
#define angle_scale_y    0.0054931641   
#define angle_scale_z    0.0054931641   

#endif


                                                                     
#define gyro_zero_x 32768
#define gyro_zero_y 32768
#define gyro_zero_z 32768
                                                                                                 
#define compass_bias_x  0
#define compass_bias_y  2
#define compass_bias_z  2

/*define on 2009.03
#define compass_scale_x  0.8340
#define compass_scale_y  0.1515
#define compass_scale_z  0.1724
*/


//  define on 2009.05.11
#define compass_scale_x  0.3125
#define compass_scale_y  0.2778
#define compass_scale_z  0.3571


extern int spi_fd;
extern int imuframe[28];
extern float imu_gyro[3];
extern float imu_accel[3];
extern float imu_magn[3];
extern float imu_angle[3];



#ifdef __cplusplus
extern "C"{
#endif
extern void transfer(int fd);
extern int SPI_init(void);
extern void take_SPI_data(int fd);
#ifdef __cplusplus
}
#endif
#endif
