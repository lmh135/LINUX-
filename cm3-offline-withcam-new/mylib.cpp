#include<iostream>
using namespace std;
#include<stdlib.h>


char *index(char*s,int c)
{
	char *line_in=s;
	while(*line_in!=c)
	{
		if(*line_in++=='\0')
			return NULL;
	}
	return line_in;
}


unsigned int CharCounter(char*s,int c)
{
	unsigned int sum=0;
	char*line_in=s;
	while(*line_in!='\0')
	{
		if(*line_in++==c)
			sum++;
	}
	return sum;
}
