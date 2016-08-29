/*************************************************************************
	> File Name: rabbit.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2016年08月28日 星期日 23时20分11秒
 ************************************************************************/

#include<stdio.h>
#include <string.h>
int main(int argc,char* argv[])
{
	if(argc!=2){
		printf("bad argment\n");
		return 1;
	}
	int child[3]= {0};       
	child[0] = 1;
	int i;
	for(i=1;i<atoi(argv[1]);i++)
		child[i%3] = child[(i+1)%3] + child[(i+2)%3];
	printf("%d\n",child[(i-1) % 3]);
	return 0;
}
