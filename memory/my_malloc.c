/*************************************************************************
	> File Name: my_malloc.c
	> Author: wyw0907
	> Mail: 827349708@qq.com 
	> Created Time: 2016年09月09日 星期五 10时25分37秒
 ************************************************************************/

#include<stdio.h>


char pool[10100];

void pool_init()
{
	char *pm = &pool[10000];
	int i = 0;
	while(i<100)
	{
		pm[i] = 0;
		i++;
	}
}

void *my_malloc(unsigned int size)
{
	char *pm = &pool[10000];
	if(size > 100)
		return NULL;
	int i = 0;
	for(i=0;i<100;i++)
	{
		if(pm[i] == 0)
		{
			pm[i] = 1;
			return pool + i * 100;
		}
	}
	if(i == 100)
		return NULL;
}

void my_free(void *p)
{
	char *pm  = &pool[10000];
	char *ps = (char *)p;
	if(ps < pool || ps > &pool[9999])
		return ;
	if((ps-pool) % 100)
		return ;
	if(pm[(ps-pool)/100] == 1)
		pm[(ps-pool)/100] = 0;
	return;
}

struct text{
	char buf[50];
};

int main()
{
	pool_init();
	printf("pool addr : %p \n",pool);
	struct text *Text ;
	Text = (struct text *)my_malloc(sizeof(struct text));
	if(Text == NULL){
		printf("malloc error!\n");
		return;
	}
	else
		printf("malloc Text!\n");
	printf("Text addr : %p \n",Text);
	if(Text != NULL)
	{	
		my_free(Text);
		printf("free Text!\n");
	}
	else
		printf("free error!\n");
	return;
}
