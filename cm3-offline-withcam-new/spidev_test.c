/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */


#include "imu.h"


#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
int spi_fd;
#if 1
int imuframe[28];
float ahrs_pqr[3];
float imu_gyro[3];
float imu_accel[3];
float imu_magn[3];
float imu_angle[3];


void imu_update( void )
{

        //---------------------------------
        // Œì²éimuframeµÄ±êÖŸÎ»
       /*
       char i,temp = 0 ;
       for(i =1 ; i !=7 ; ++i)
          if((imuframe[i] & 0x4000) != 0)
               temp ++;
       if(imuframe[7] != 0)
            temp ++;
       if(temp >= 1){
           imu_update_flag = 3;    
           return ;
       }*/
/*
	static double yaw0 = 0;

	double gx,gy,gz;
	double ax,ay,az;
	double mx,my,mz;
	double roll,pitch,yaw;
	double rol,pit,yw;
	double Hx,Hy;
*/

	char i=0;
       	float rate ;
#if ADIS16365
	 rate = imuframe[ INDEX_GX ]&0x3FFF;	
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x1fff)rate=rate-0x4000;
	ahrs_pqr[ 0 ] = rate * gyro_scale_x  ;
		
        rate = imuframe[ INDEX_GY ]&0x3FFF;
	//rate = imuframe[ INDEX_GY] - gyro_zero_y;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x1fff)rate=rate-0x4000;
	ahrs_pqr[ 1 ] = rate * gyro_scale_y  ;
	
        rate = imuframe[ INDEX_GZ ]&0x3FFF;
	//rate = imuframe[ INDEX_GZ ] - gyro_zero_z;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x1fff)rate=rate-0x4000;
	ahrs_pqr[ 2 ] = rate * gyro_scale_z ;
	
        rate = imuframe[ INDEX_AX ]&0x3FFF;
	//rate = imuframe[ INDEX_AX ] - bias_ax;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x1fff)rate=rate-0x4000;
        imu_accel[ 0 ] = rate * accel_scale_x;
  
        rate = imuframe[ INDEX_AY ]&0x3FFF;
	//rate = imuframe[ INDEX_AY ] - bias_ay;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x1fff)rate=rate-0x4000;
	imu_accel[ 1 ] = rate * accel_scale_y;
        
	rate = imuframe[ INDEX_AZ ]&0x3FFF;    
	//rate = imuframe[ INDEX_AZ ] - bias_az;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x1fff)rate=rate-0x4000;
	imu_accel[ 2 ] = rate * accel_scale_z;
        
       
       for(i=0 ; i<3 ; i++ )
	{
	printf("%.3f ",ahrs_pqr[i])  ;
	//ahrs_pqr[i] = limit(ahrs_pqr[i],-350.0,350.0);
	} 

	for(i=0 ; i<3 ; i++ )
	{
	 printf("%.3f ",imu_accel[i])  ;
    	 //imu_accel[i] = limit(imu_accel[i],-40.0,40.0);
	}   
	printf("\n");
#endif		


#if ADIS16480
	
       rate = ((imuframe[ INDEX_GX ]&0xFF)<<8)+(imuframe[ INDEX_GX+1 ]&0xFF);	
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x57E4)rate=rate-0x10000;
	imu_gyro[ 0 ] = rate * gyro_scale_x  ;
		
        rate = ((imuframe[ INDEX_GY ]&0xFF)<<8)+(imuframe[ INDEX_GY+1 ]&0xFF);
	//rate = imuframe[ INDEX_GY] - gyro_zero_y;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x57E4)rate=rate-0x10000;
	imu_gyro[ 1 ] = rate * gyro_scale_y  ;
	
        rate = ((imuframe[ INDEX_GZ ]&0xFF)<<8)+(imuframe[ INDEX_GZ+1 ]&0xFF);
	//rate = imuframe[ INDEX_GZ ] - gyro_zero_z;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x57E4)rate=rate-0x10000;
	imu_gyro[ 2 ] = rate * gyro_scale_z ;
        


        rate = ((imuframe[ INDEX_AX ]&0xFF)<<8)+(imuframe[ INDEX_AX+1 ]&0xFF);
	//rate = imuframe[ INDEX_AX ] - bias_ax;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x30d4)
	{	
		rate=rate-0x10000;
	}
        imu_accel[ 0 ] = rate * accel_scale_x;

	rate = ((imuframe[ INDEX_AY ]&0xFF)<<8)+(imuframe[ INDEX_AY+1 ]&0xFF);
	//rate = imuframe[ INDEX_AY ] - bias_ay;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x30d4)
	{	
		rate=rate-0x10000;
	}
	imu_accel[ 1 ] = rate * accel_scale_y;
        
	rate = ((imuframe[ INDEX_AZ ]&0xFF)<<8)+(imuframe[ INDEX_AZ+1 ]&0xFF);  
	//rate = imuframe[ INDEX_AZ ] - bias_az;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x30d4)
	{	
		rate=rate-0x10000;
	}
	imu_accel[ 2 ] = rate * accel_scale_z;

 
	rate = ((imuframe[ INDEX_MX ]&0xFF)<<8)+(imuframe[ INDEX_MX+1 ]&0xFF);  
	//rate = imuframe[ INDEX_AZ ] - bias_az;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x7fff)rate=rate-0x10000;
	imu_magn[ 0 ] = rate * magn_scale_x;
        
        rate = ((imuframe[ INDEX_MY ]&0xFF)<<8)+(imuframe[ INDEX_MY+1 ]&0xFF);   
	//rate = imuframe[ INDEX_AZ ] - bias_az;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x7fff)rate=rate-0x10000;
	imu_magn[ 1 ] = rate * magn_scale_y;
        
        rate = ((imuframe[ INDEX_MZ ]&0xFF)<<8)+(imuframe[ INDEX_MZ+1 ]&0xFF);   
	//rate = imuframe[ INDEX_AZ ] - bias_az;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x7fff)rate=rate-0x10000;
	imu_magn[ 2 ] = rate * magn_scale_z;


	rate = ((imuframe[ INDEX_ROLL ]&0xFF)<<8)+(imuframe[ INDEX_ROLL+1 ]&0xFF);    
	//rate = imuframe[ INDEX_AZ ] - bias_az;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x7fff)rate=rate-0x10000;
	imu_angle[ 0 ] = rate * angle_scale_x;     //pitch = -imu_angle[ 0 ]
        
        rate = ((imuframe[ INDEX_PITCH ]&0xFF)<<8)+(imuframe[ INDEX_PITCH+1 ]&0xFF);
	//rate = imuframe[ INDEX_AZ ] - bias_az;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x3fff)rate=rate-0x10000;
	imu_angle[ 1 ] = rate * angle_scale_y;     //roll = -imu_angle[ 1 ]
        
        rate = ((imuframe[ INDEX_YAW ]&0xFF)<<8)+(imuframe[ INDEX_YAW+1 ]&0xFF);
	//rate = imuframe[ INDEX_AZ ] - bias_az;
        //if(rate>0x2fff)rate=rate-0x4000;
        if(rate>0x7fff)rate=rate-0x10000;
	imu_angle[ 2 ] = rate * angle_scale_z;     //yaw = -imu_angle[ 2 ] 加修正



	




        //all magu must to change to angle
        
        
        //²âÊÔÓÃ
        //ahrs_pqr[ 0 ]=ahrs_pqr[ 0 ]+245;
        //ahrs_pqr[ 1 ]=ahrs_pqr[ 1 ]+240;
   /*     
        ahrs_pqr[ 0 ]=ahrs_pqr[ 0 ]*cos(imu_magn[ 1 ]/57.2957768)+ahrs_pqr[ 1 ]*sin(imu_magn[ 1 ]/57.2957768)*sin(imu_magn[ 0 ]/57.2957768)-ahrs_pqr[ 2 ]*cos(imu_magn[ 0 ]/57.2957768)*sin(imu_magn[ 1 ]/57.2957768);
        ahrs_pqr[ 1 ]=ahrs_pqr[ 1 ]*cos(imu_magn[ 0 ]/57.2957768)+ahrs_pqr[ 2 ]*sin(imu_magn[ 0 ]/57.2957768);
        
  if(ahrs_pqr[ 1 ]>=0)
        {if(ahrs_pqr[ 0 ]>=0)imu_accel[ 0 ]=57.2957768*atan(ahrs_pqr[ 1 ]/ahrs_pqr[ 0 ]);
        else imu_accel[ 0 ]=180+57.2957768*atan(ahrs_pqr[ 1 ]/ahrs_pqr[ 0 ]);
        }
        
        if(ahrs_pqr[ 1 ]<0)
        {if(ahrs_pqr[ 0 ]>=0)imu_accel[ 0 ]=57.2957768*atan(ahrs_pqr[ 1 ]/ahrs_pqr[ 0 ]);
        else imu_accel[ 0 ]=-180+57.2957768*atan(ahrs_pqr[ 1 ]/ahrs_pqr[ 0 ]);
        }
        imu_accel[ 0 ]=-imu_accel[ 0 ];//²âÊÔœáÊø
  
  */
  
       
        
       
          
    /*    
       //²âÊÔ,ÒòÎªimuµÄÊÇyÖáÖžÏò±±£¬ËùÒÔÒªŒÓÉÏ90¶È¡£
        if(imu_magn[ 2 ]<=90) imu_magn[ 2 ]=imu_magn[ 2 ]+90;
        else imu_magn[ 2 ]=imu_magn[ 2 ]-270;
        //by xiong
*/
/*
       for(i=0 ; i<3 ; i++ )
	{
    	 imu_accel[i] = limit(imu_accel[i],-198.0,198.0);
	}   
       for(i=0 ; i<3 ; i++ )
	{
	ahrs_pqr[i] = limit(ahrs_pqr[i],-32767.0,32767.0);
	}   
       for(i=0 ; i<3 ; i++ )
        {
        imu_magn[i] = limit(imu_magn[i],-180.0,180.0);
        }   
*/



/*
	gx = imu_gyro[0];
	gy = -imu_gyro[1];
	gz = -imu_gyro[2];
	

	ax = imu_accel[0];
	ay = -imu_accel[1];
	az = -imu_accel[2];

	mx = imu_magn[0];
	my = -imu_magn[1];
	mz = -imu_magn[2];

	roll = imu_angle[0];
	pitch = -imu_angle[1];
	yaw = -imu_angle[2];	



	imu_angle[0] = roll;
	imu_angle[1] = pitch;
	imu_angle[2] = yaw;
*/


/*
	rol = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)));		
	pit= atan(-ax / sqrt(pow(ay, 2) + pow(az, 2)));
	Hx = mx*cos(pit) + my*sin(pit)*sin(rol) + mz*sin(pit)*cos(rol);
	Hy = my*cos(rol) - mz*sin(rol);
	yw = atan2(Hx, Hy);
*/

	//printf("gx:%.3f  gy:%.3f  gz:%.3f \n",gx,gy,gz);
	//printf("ax:%.3f  ay:%.3f  az:%.3f \n",ax,ay,az);
	//printf("mx:%.3f  my:%.3f  mz:%.3f \n",mx,my,mz);
	//printf("roll:%.3f  pitch:%.3f  yaw:%.3f \n",imu_angle[0],-imu_angle[1],-imu_angle[2]);
	//printf("rol:%.3f  pit:%.3f  yw:%.3f",rol,pit,yw);
/*
       for(i=0 ; i<3 ; i++ )
	{
	printf("%.3f ",imu_gyro[i])  ;
	//ahrs_pqr[i] = limit(ahrs_pqr[i],-350.0,350.0);
	} 

       for(i=0 ; i<3 ; i++ )
	{
	 printf("%f ",imu_accel[i])  ;
    	 //imu_accel[i] = limit(imu_accel[i],-40.0,40.0);
	}  

	for(i=0 ; i<3 ; i++ )
	{
	 printf("%.3f ",imu_magn[i])  ;
    	 //imu_accel[i] = limit(imu_accel[i],-40.0,40.0);
	}   

	//for(i=0 ; i<3 ; i++ )
	{
	 printf("%.3f ",imu_angle[i])  ;
    	 //imu_accel[i] = limit(imu_accel[i],-40.0,40.0);
	}   
*/
	//printf("\n");
#endif		
	
} 

#endif


static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev0.0";
static uint8_t mode=3;
static uint8_t bits = 8;
static uint32_t speed = 10000000;
static uint16_t delay=0;

void transfer(int fd)
{
	int ret;
#if ADIS16365
	uint16_t tx[] = {0x0200,0x0400,0x0600,0x0800,0x0A00,0x0C00,0x0E00,0x3C00};
	uint16_t rx[ARRAY_SIZE(tx)] = {0,0,0,0,0,0,0,0 };
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len =ARRAY_SIZE(tx)*2,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
        ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
	//printf("ARRAY_SIZE(tx)=%d\n",ARRAY_SIZE(tx));
	for (ret = 0; ret <ARRAY_SIZE(tx); ret++) {
		//if (!(ret % 6))
		//	puts("");
		if(ret!=0)
		imuframe[ret-1]=rx[ret];
		else
		imuframe[7]=rx[ret];
		//printf("%.4X ", rx[ret]); 
	}
	puts("");

#endif

#if ADIS16480
        uint8_t tx[] = {0x12,0x00,0x16,0x00,0x1A,0x00,0x1E,0x00,0x22,0x00,0x26,0x00,0x28,0x00,0x2A,0x00,0x2C,0x00,0x6A,0x00,0x6C,0x00,0x6E,0x00,0x80,0x00,0x80,0x00};
	//uint8_t tx[] = {0x80,0x00,0x1E,0x00,0x22,0x00,0x26,0x00,0x80,0x00,0x80,0x00};
	//uint8_t tx1[] = {0x80,0x00};uint8_t tx2[] = {0x12,0x00};uint8_t tx3[] = {0x16,0x00};uint8_t tx4[] = {0x1A,0x00};uint8_t tx5[] = {0x80,0x00};
	uint8_t rx[ARRAY_SIZE(tx)] = {0,0,};
	
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len =ARRAY_SIZE(tx),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
		//.cs_change = 0,
	};
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
	//printf("ARRAY_SIZE(tx)=%d\n",ARRAY_SIZE(tx));

	for (ret = 0; ret <ARRAY_SIZE(tx); ret++) {
		//if (!(ret % 6))
		//	puts("");
		if(ret!=0)
		imuframe[ret-1]=rx[ret];
		else
		imuframe[27]=rx[ret];
		//printf("%.4X ", rx[ret]); 
	}

/*
	struct spi_ioc_transfer tr2 = {
		.tx_buf = (unsigned long)tx2,
		.rx_buf = (unsigned long)rx,
		.len =ARRAY_SIZE(tx2),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
		//.cs_change = 0,
	};
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr2);
	if (ret < 1)
		pabort("can't send spi message");
	//printf("ARRAY_SIZE(tx)=%d\n",ARRAY_SIZE(tx));
	for (ret = 0; ret <ARRAY_SIZE(tx2); ret++) {
		//if (!(ret % 6))
		//	puts("");
		if(ret!=0)
		imuframe[0]=rx[ret];
		//else
		//imuframe[9]=rx[ret];
		//printf("%.4X ", rx[ret]); 
	}

	struct spi_ioc_transfer tr3 = {
		.tx_buf = (unsigned long)tx3,
		.rx_buf = (unsigned long)rx,
		.len =ARRAY_SIZE(tx3),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
		//.cs_change = 0,
	};
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr3);
	if (ret < 1)
		pabort("can't send spi message");
	//printf("ARRAY_SIZE(tx)=%d\n",ARRAY_SIZE(tx));
	for (ret = 0; ret <ARRAY_SIZE(tx3); ret++) {
		//if (!(ret % 6))
		//	puts("");
		if(ret!=0)
		imuframe[2]=rx[ret];
		else
		imuframe[1]=rx[ret];
		//printf("%.4X ", rx[ret]); 
	}

	struct spi_ioc_transfer tr4 = {
		.tx_buf = (unsigned long)tx4,
		.rx_buf = (unsigned long)rx,
		.len =ARRAY_SIZE(tx4),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
		//.cs_change = 0,
	};
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr4);
	if (ret < 1)
		pabort("can't send spi message");
	//printf("ARRAY_SIZE(tx)=%d\n",ARRAY_SIZE(tx));
	for (ret = 0; ret <ARRAY_SIZE(tx4); ret++) {
		//if (!(ret % 6))
		//	puts("");
		if(ret!=0)
		imuframe[4]=rx[ret];
		else
		imuframe[3]=rx[ret];
		//printf("%.4X ", rx[ret]); 
	}



	struct spi_ioc_transfer tr5 = {
		.tx_buf = (unsigned long)tx5,
		.rx_buf = (unsigned long)rx,
		.len =ARRAY_SIZE(tx5),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
		//.cs_change = 0,
	};
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr5);
	if (ret < 1)
		pabort("can't send spi message");
	//printf("ARRAY_SIZE(tx)=%d\n",ARRAY_SIZE(tx));
	for (ret = 0; ret <ARRAY_SIZE(tx5); ret++) {
		//if (!(ret % 6))
		//	puts("");
		if(ret!=0)
		//imuframe[2]=rx[ret];
			;
		else
		imuframe[5]=rx[ret];
		//printf("%.4X ", rx[ret]); 
	}
*/
#endif

	
	
}

void take_SPI_data(int fd)
{
	transfer(fd);
	imu_update();
}



static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
	     "  -s --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word \n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 's':
			speed = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'b':
			bits = atoi(optarg);
			break;
		case 'l':
			mode |= SPI_LOOP;
			break;
		case 'H':
			mode |= SPI_CPHA;
			break;
		case 'O':
			mode |= SPI_CPOL;
			break;
		case 'L':
			mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			mode |= SPI_CS_HIGH;
			break;
		case '3':
			mode |= SPI_3WIRE;
			break;
		case 'N':
			mode |= SPI_NO_CS;
			break;
		case 'R':
			mode |= SPI_READY;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

int SPI_init(void)
{
	int ret = 0;
	spi_fd = open(device, O_RDWR);
	if (spi_fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	return ret;
}


/*
int main(int argc, char *argv[])
{	
	
	parse_opts(argc, argv);
	SPI_init();
	while(1)
	{
		take_SPI_data(fd);
		usleep(100000);
	}

	close(fd);

	
}
*/
