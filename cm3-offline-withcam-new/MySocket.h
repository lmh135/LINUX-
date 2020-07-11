#ifndef __MY_SOCKEt_H
#define __MY_SOCKEt_H
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<iostream>
using namespace std;
#define MAXLINE   4096

class MySocket
{
	private:
	struct sockaddr_in address;
	int sockfd;

	
	 
	public:
	MySocket()
	{ memset(&address,0,sizeof(address));}
	int createClient(const char*protocol,const char* IPAddress,int Port,const char* client_name);
	int createServer(const char*protocol,const char* IPAddress,int Port,const char* server_name);
	int static send_msg(int sockfd1,const char* buf);
	int static send_nmsg(int sockfd,char* buf,unsigned int len);
	int static recv_msg(int sockfd1,char* buf);
};


class Socket
{
	public:
		Socket();
		int UDPCreateServer(const char * Addr_family, const char * Protocol, \
		const char * IPAddr, const int Port);
		
		int UDPCreateClient(const char * Addr_family, const char * Protocol,\
		const char * IPAddr, const int Port);
		
		int UDPReceivePackets(void * buf, int len);
		
	 	int UDPSendPackets(void * buf, int len);
		
		int GetSocketFD();
		
		//~Socket();
	private:
		int socket_fd;
		struct sockaddr_in SocketAddr;
		socklen_t SocketAddrlength;
};

#endif
