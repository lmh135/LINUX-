#include<stdio.h>
#include"dataprocess.h"
double Cbn[3][3];
Point PointXYZ[12][16];
void generateCbn(double roll,double pitch,double yaw)
{
    double cosPitch,sinPitch;
    double cosRoll,sinRoll;
    double cosYaw,sinYaw;
    cosPitch = cos((pitch)*MDEG2RAD);
    sinPitch = sin((pitch)*MDEG2RAD);

    cosRoll = cos((roll)*MDEG2RAD);
    sinRoll = sin((roll)*MDEG2RAD);

    cosYaw = cos((yaw)*MDEG2RAD);
    sinYaw = sin((yaw)*MDEG2RAD);

    Cbn[0][0] = cosYaw*cosPitch;
    Cbn[0][1] = -sinYaw*cosRoll + cosYaw*sinPitch*sinRoll;
    Cbn[0][2] = sinYaw*sinRoll + cosYaw*sinPitch*cosRoll;

    Cbn[1][0] = sinYaw*cosPitch;
    Cbn[1][1] = cosYaw*cosRoll + sinYaw*sinPitch*sinRoll;
    Cbn[1][2] = sinYaw*sinPitch*cosRoll - cosYaw*sinRoll;

    Cbn[2][0] = -sinPitch;
    Cbn[2][1] = cosPitch*sinRoll;
    Cbn[2][2] = cosPitch*cosRoll;

}

void dataHandle (Point point[][16],unsigned int scan_idx, double posNED[], double theta[])
{
       generateCbn(theta[0],theta[1],theta[2]);
       int i,j, point_num;
       for(i=0;i<12;i++)
	   for(j=0;j<16;j++)
           {
             PointXYZ[i][j].x = point[i][j].x*Cbn[0][0]+point[i][j].y*Cbn[0][1]+point[i][j].z*Cbn[0][2] + posNED[0];
             PointXYZ[i][j].y = point[i][j].x*Cbn[1][0]+point[i][j].y*Cbn[1][1]+point[i][j].z*Cbn[1][2] + posNED[1];
             PointXYZ[i][j].z = point[i][j].x*Cbn[2][0]+point[i][j].y*Cbn[2][1]+point[i][j].z*Cbn[2][2] + posNED[2];
	   }
              
}
/*
extern int count_num;
extern int camcount;
extern int ggacount;
extern int vtgcount;

bool lazer_imu_synchronization(Packet *data)
{
	int i,j,k;
	unsigned long tim_lazer,tim_imu1,tim_imu2;
	printf("roll:%.3f  pitch:%.3f yaw:%.3f \n",imu_data[0].imu_angle[0],imu_data[0].imu_angle[1],imu_data[0].imu_angle[2]);

	for(i=0;i<12;i++)
	{
		tim_lazer = channel[i].time_us;
		//printf("%ld ",tim_lazer);
		for(j=0;j<10;j++)
		{
			tim_imu1 = imu_data[j].time_us;
			if(tim_imu1 <= tim_lazer) break;
		}
		if(j==0)
		{

			data->lazer_data[i].Azimuth = channel[i].Azimuth;
			data->lazer_data[i].time_us = channel[i].time_us;
			for(k=0;k<16;k++)
			{
				data->lazer_data[i].distance[k] = channel[i].distance[k];
			}


			data->imu_data[i] = imu_data[j];
		}
		else if(j<10)
		{
			tim_imu2 = imu_data[j-1].time_us;

			for(k=0;k<16;k++)
			{
				data->lazer_data[i].distance[k] = channel[i].distance[k];
			}
			
			data->lazer_data[i].Azimuth = channel[i].Azimuth;
			data->lazer_data[i].time_us = channel[i].time_us;

			data->imu_data[i].time_us = channel[i].time_us;

			for(k=0;k<3;k++)
			{
				//data->imu_data[i].imu_gyro[k] = imu_data[j].imu_gyro[k]+(tim_lazer-tim_imu1)*(imu_data[j-1].imu_gyro[k]-imu_data[j].imu_gyro[k])/(tim_imu2-tim_imu1);
				//data->imu_data[i].imu_accel[k] = imu_data[j].imu_accel[k]+(tim_lazer-tim_imu1)*(imu_data[j-1].imu_accel[k]-imu_data[j].imu_accel[k])/(tim_imu2-tim_imu1);
				//data->imu_data[i].imu_magn[k] = imu_data[j].imu_magn[k]+(tim_lazer-tim_imu1)*(imu_data[j-1].imu_magn[k]-imu_data[j].imu_magn[k])/(tim_imu2-tim_imu1);
				data->imu_data[i].imu_angle[k] = imu_data[j].imu_angle[k]+(tim_lazer-tim_imu1)*(imu_data[j-1].imu_angle[k]-imu_data[j].imu_angle[k])/(tim_imu2-tim_imu1);
			}

		}
		else
		{
			printf("synchronization failed! \n");
			return false;
		}



	}
	//printf(" \n");
	for(k=0;k<3;k++)
	{
		data->gps_data.ned_xyz[k] = position.current_xyz[k];
		data->gps_data.ned_uvw[k] = position.current_uvw[k];
	}
	data->gps_data.gps_mod = gps_gga_quality;
	data->gps_data.gps_sats = gps_gga_num_sats;
	data->gps_data.gps_heading = gps_heading;
	data->gps_data.ggacount = ggacount;
	data->gps_data.vtgcount = vtgcount;
	data->gps_data.headingcount = headingcount;
	data->round_count = count_num;
	data->cam_count = camcount;


	return true;

}

*/



















