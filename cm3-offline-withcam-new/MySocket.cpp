#include "MySocket.h"
int MySocket::createClient(const char* protocol,const char* IPAddress,int Port,const char* client_name)
{	
	//printf("%s",protocol);
	//if("TCP"==protocol)//只能比较地址
   	if(!strcmp("TCP",protocol))
	{
		if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
          	{
	     		printf("%s creat socket error:%s(error:%d)\n",client_name,strerror(errno),errno);
	     		return -1;
        	}
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(IPAddress);
		address.sin_port =htons(Port);
		int result = connect(sockfd,(struct sockaddr*)&address,sizeof(address));
		if(result==-1)
		{
			printf("%s connect error:%s(error:%d)\n",client_name,strerror(errno),errno);
			return -1;
		}
		else
		{
			printf("%s client creater sucuess!\n",client_name);
			return sockfd;
		}
	}
	else
	{	
		printf("Can only create TCP Client Socket\n");
		return -1;
	}
}


int MySocket::createServer(const char*protocol,const char* IPAddress,int Port,const char* server_name)
{
	if(!strcmp("TCP",protocol))
	{
		if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
          	{
	     		printf("%s creat socket error:%s(error:%d)\n",server_name,strerror(errno),errno);
	     		return -1;
        	}
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(IPAddress);
		address.sin_port =htons(Port);
		int result = bind(sockfd,(struct sockaddr*)&address,sizeof(address));
		if(result==-1)
		{
			printf("%s bind error:%s(error:%d)\n",server_name,strerror(errno),errno);
			return -1;
		}
		else
		{
			printf("%s server creater sucuess!\n",server_name);
			return sockfd;
		}
		
	}
	else
	{	
		printf("Can only create TCP Server Socket");
		return -1;
	}
}


int MySocket::send_msg(int sockfd,const char* buf)
{
	if(send(sockfd,buf,strlen(buf),0)<0)
        {
		 printf("send msg error:%s(error:%d)\n",strerror(errno),errno);
	     	 return -1;
        }
	else 
		return 0;
}

int MySocket::send_nmsg(int sockfd,char* buf,unsigned int len)
{
	if(send(sockfd,buf,len,0)<0)
        {
		 printf("send nmsg error:%s(error:%d)\n",strerror(errno),errno);
	     	 return -1;
        }
	else 
		return 0;
}

int MySocket::recv_msg(int sockfd,char* buf)
{
	int recv_len=recv(sockfd,buf,MAXLINE,0);	
	if(recv_len==-1)
        {
		 printf("recv error");
	     	 return -1;
        }
	
	else 
	{	
		buf[recv_len]='\0';
		return 0;
	}
}






//UDPSocket//old system

Socket::Socket():socket_fd(0),SocketAddrlength(0)
{
	bzero(&SocketAddr,sizeof(SocketAddr));
}

int Socket::UDPCreateServer(const char * Addr_family,const char * Protocol, \
							const char * IPAddr , const int Port)
{
	if ((!strncmp(Addr_family,"IPV4",5)) && (!strncmp(Protocol,"UDP",4)))
		{
			if((socket_fd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
				perror("socket created error");
                cout<<"UDPCreateServer ok! fd = "<<socket_fd<<endl;  //by zhao
		}
	bzero(&SocketAddr,sizeof(SocketAddr));
	SocketAddr.sin_family = AF_INET,
	SocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	SocketAddr.sin_port = htons(Port);
	SocketAddrlength = sizeof(SocketAddr);
	bind(socket_fd,(struct sockaddr *)&SocketAddr,SocketAddrlength);	

	return 0;
}

int Socket::UDPCreateClient(const char * Addr_family, const char *Protocol, \
							const char *IPAddr,const int Port)
{
	if ((!strncmp(Addr_family,"IPV4",5)) && (!strncmp(Protocol,"UDP",4)))
		{
			if((socket_fd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
				perror("socket created error");
            cout<<"UDPCreateClient ok! fd = "<<socket_fd<<endl;  //by zhao
		}
	bzero(&SocketAddr,sizeof(SocketAddr));
	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_addr.s_addr = inet_addr(IPAddr);
	SocketAddr.sin_port = htons(Port);
	SocketAddrlength = sizeof(SocketAddr);	
	return 0;
}

int Socket::UDPReceivePackets(void * buf,int buf_len )
{
	if((recvfrom(socket_fd,buf,buf_len,0,(struct sockaddr *)&SocketAddr,&SocketAddrlength)) == -1)
	perror("UDP data receive error");
	return 0;
}

int Socket::UDPSendPackets( void * buf,int buf_len)
{
	if( (sendto(socket_fd,buf,buf_len,0,(struct sockaddr *)&SocketAddr,SocketAddrlength)) == -1 )
	perror("UDP data send error");
	return 0;
}

int Socket::GetSocketFD(void)
{
	return this->socket_fd;
}




