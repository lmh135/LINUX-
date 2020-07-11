//死锁代码 加锁顺序一致可以解决死锁问题
#include <stdio.h>    
#include <stdlib.h>    
#include <string.h>    
#include <unistd.h>    
#include <pthread.h>    
#include <string>
#include <iostream>
using namespace std;

int ticket = 100;    
pthread_mutex_t mutex1;    
pthread_mutex_t mutex2;    

void* route1(void* arg)    
{       
	string id=*(string*)arg;	
	while(1)    
	{    
		pthread_mutex_lock(&mutex1);    
		if(ticket>0)    
		{    
			sleep(1);  
			cout<<id<<" sells ticket:"<<ticket<<endl;			    
			ticket--;    
		}    
		pthread_mutex_lock(&mutex2);    
		if(ticket>0)    
		{    
			sleep(1); 
			cout<<id<<" sells ticket:"<<ticket<<endl;
			ticket--;
		}
		pthread_mutex_unlock(&mutex1);
		pthread_mutex_unlock(&mutex2);
		sleep(3);
	}
	return NULL;
}
void* route2(void* arg)
{
	char* id = (char*)arg;
	while(1)
	{
		//pthread_mutex_lock(&mutex2);  // 加锁顺序不一致时，会构成死锁
		pthread_mutex_lock(&mutex1);  //解决方法
		if(ticket>0)
		{
			sleep(1);
			printf("%s sells ticket:%d\n",id,ticket);
			ticket--;
		}                                                                                                                                          
		//pthread_mutex_lock(&mutex1); // 加锁顺序不一致时，会构成死锁。
		pthread_mutex_lock(&mutex2);  //解决方法
		if(ticket>0)
		{
			sleep(1);
			printf("%s sells ticket:%d\n",id,ticket);
			ticket--;
		}
		pthread_mutex_unlock(&mutex1);
		pthread_mutex_unlock(&mutex2);
		sleep(3);
	}
	return NULL;
}

int main()
{
	pthread_t t1,t2;
	string mesg1="thread 1";
	char* mesg2="thread 2";
	pthread_create(&t1,NULL,route1,&mesg1);
	pthread_create(&t2,NULL,route2,mesg2);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
	return 0;
}          
