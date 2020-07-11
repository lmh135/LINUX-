//
//线程创建函数：
//int pthread_create(pthread_t *restrict thread,  /*线程id*/
//	const pthread_attr_t *restrict attr,    /*线程属性，默认可置为NULL，表示线程属性取缺省值*/
//	void *(*start_routine)(void*),  /*线程入口函数*/ 
//	void *restrict arg  /*线程入口函数的参数*/
//	);

/*
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

pthread_mutex_t data_mutex;
int num=0;

void* thread_func(void* arg) 
{
	for (int i = 0; i< 100000; ++i)
	{
		pthread_mutex_lock(&data_mutex);//加锁
        	num += 1;
		pthread_mutex_unlock(&data_mutex);
    	}
	return NULL;
}


int main()
{
	pthread_mutex_init(&data_mutex,NULL);
	pthread_t pt1,pt2;
	if (pthread_create(&pt1, NULL,thread_func, NULL) != 0) 
	{
	        printf("pthread_create error.");
	        exit(EXIT_FAILURE);
	}
	if (pthread_create(&pt2, NULL,thread_func, NULL) != 0) 
	{
	        printf("pthread_create error.");
	        exit(EXIT_FAILURE);
	}		

	pthread_join(pt1,NULL);//pthread_join会让主线程阻塞，直到pt1线程退出
	pthread_join(pt2,NULL);

	pthread_mutex_destroy(&data_mutex);
	cout<<num<<endl;
	return 0;
}
*/


//通过线程入口函数的参数传递互斥锁

#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<pthread.h>
using namespace std;
int num = 0;

void* thread_func(void* arg) 
{
	pthread_mutex_t* p_mutex = (pthread_mutex_t*)arg;
	for (int i = 0; i< 100000; ++i)
	{
		pthread_mutex_lock(p_mutex);
		num += 1;
		pthread_mutex_unlock(p_mutex);
	}
	return NULL;
}

int main()
{
	pthread_mutex_t m_mutex;
	pthread_mutex_init(&m_mutex, NULL);

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, thread_func, &m_mutex);
	pthread_create(&tid2, NULL, thread_func, &m_mutex);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	pthread_mutex_destroy(&m_mutex);
	cout<<num<<endl;
	return 0;
}

