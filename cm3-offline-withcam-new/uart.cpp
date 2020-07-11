#include"uart.h"

bool UartReadByets(int fd,char *pbuf,int size)
{
    int ret,left,bytes;

    left = size;

    fd_set fds;
    struct timeval timeout;

    while(left>0)
    {
        ret = 0;
        bytes = 0;
        FD_ZERO(&fds);
        FD_SET(fd,&fds);

        memset(&timeout, 0, sizeof(timeout));
        //timeout.tv_usec = 100;
	

       // int rc = select(fd+1, &fds, 0, 0, (timeout_val > 0) ? &timeout : 0);
       // ioctl(fd, FIONREAD, &bytes);
        //bytes = select(fd+1, &fds, 0, 0, &timeout);
	bytes = select(fd+1, &fds, 0, 0, NULL);

        if(bytes>0)
        {
            ret = read(fd,pbuf,left);
        }

        if(ret >0)
        {
            left -= ret;
            pbuf += ret;
        }
        //usleep(100);//shi
    }
    if(left ==0)
        return true;
    else
        return false;

}

#if 1
bool UartGetData(char buf[], int fd)
{
	char *temp;

	int length;
    do
    {
            if (!UartReadByets(fd, &buf[0], 1))
                    return false;
    }while(buf[0] != '$');
    //printf("get $\n");

    int i;
    for (i = 1; i < 5; i++)
    {
            if (!UartReadByets(fd, &buf[i], 1))
                    return false;
    }

    if (buf[0] != '$' || buf[1] != 'D' || buf[2] != 'A'
                    || buf[3] != 'T' || buf[4] != 'A'){
            perror("serial_get_data: no '$DATA' received!");
            return false;
    }
/*

	if (!UartReadByets(fd, &buf[5], 1))
         return false;
	
	for (i = 6; i < 10; i++)
    {
            if (!UartReadByets(fd, &buf[i], 1))
                    return false;
    }
	
	temp = buf;
	printf("%s	",temp);
	temp = index(temp,',');
	temp++;

	length = strtol(temp,&temp,10);
	printf("%d\n",length);

    i = 10;
*/
	i=5;
    do
    {

        if(i < 1155)
        {
              if (!UartReadByets(fd, &buf[i], 50))
              {
              printf("read 10 bytes failed \n");
              return false;
              }
            i+=50;
			//printf("50:%d	",i);
        }

        if(i >= 1155)
        {
        //i++;
            if (!UartReadByets(fd, &buf[i], 1))
            {
              printf("read 1 bytes failed \n");
              return false;
            }

            i++;
            //printf("%d ",i);
            //if(i<200)
            //{
                //printf("%d   %c",i,buf[i]);
            //}
//              cout<<"read 1 suc!"<<endl;
        //	i++;
        }
        /*
        if(i > 350)
        {
            //printf("out of 350\n");
            break;
        }
        */
      if (i > 2048){
              //cout << "exceed 1024"<< endl;
              perror("serial_get_data: exceed 1024 bytes without end!");
              return false;
      }
    }while(i < 1285);    //by zhao
    //}while((buf[i] != '\r')&&(buf[i-1] != '\n'));
   // printf("%d   %c\n",i,buf[i]);
   //printf("%s\n",buf);

	return true;


}
#endif
int serial_init(const char *serial_dev1,  int & fd)
{  //cout<<"start serial"<<endl;
 	fd = open( serial_dev1, O_RDWR|O_NOCTTY );
	if( fd == -1 ){
		perror( serial_dev1 );
		return -1;
	}

	struct termios term1;
	if(tcgetattr( fd , &term1 ) < 0){
		printf("tcgetattr error \n");
		return -1;
	}
	term1.c_lflag &= ~( ECHO | ICANON | IEXTEN | ISIG );
	term1.c_iflag &= ~( BRKINT | ICRNL | INPCK | ISTRIP | IXON );
	term1.c_cflag &= ~( CSIZE | PARENB );
	term1.c_cflag |= CS8;
	term1.c_oflag &= ~( OPOST );

	cfsetispeed( &term1,B115200 );//set baud rate for imu and gps modules
	cfsetospeed( &term1,B115200);
	
	if( tcsetattr( fd, TCSANOW, &term1 )<0){
		perror("tcsetattr");
		return -1;
	}
	return 0;
}

/*
int main()
{
    char buf[100];
    int fd,ret;
        
	if(serial_init("/dev/ttyO0",fd)<0)
	{
	cout<<"ttyO0 port init error!"<<endl;
	return 1;
	} 
 
	cout<<"open uart successfully!"<<endl;
   while(1)
    {
     memset(buf,0,sizeof(buf));
    //if (!UartReadByets(fd, &buf[0], 1))
    //return false;
    ret = read(fd,buf,1);
     printf("%s",buf);
    // printf("########################################\n");
     //usleep(10000);
    }
    close(fd);
}
*/
