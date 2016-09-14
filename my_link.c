/*************************************************************************
	> File Name: my_link.c
	> Author: wyw0907
	> Mail: 827349708@qq.com 
	> Created Time: 2016年09月09日 星期五 11时17分54秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_link.h"

link_head *head_init(void)
{
	link_head *p = (link_head *)malloc(sizeof(link_head));
	if(p == NULL)
		return NULL;
	p->next = NULL;
	return p;
}

link_node *node_create(data_t data)
{
	link_node *p  = (link_node *)malloc(sizeof(link_node));
	if(p == NULL)
		return NULL;
	p->next = NULL;
	strcpy(p->data.str,data.str);
	return p;
}
int link_insert(link_head *head,link_node *prevnode,link_node *pnode)
{
	if(head == NULL || prevnode == NULL || pnode ==NULL || head->next ==NULL)
		return -1;
	if(link_isset(head,pnode) || !link_isset(head,prevnode))
		return -1;
	pnode->next = prevnode->next;
	prevnode->next = pnode;
	return 0;
}
int link_insert_tail(link_head *head,link_node *pnode)
{
	if(head == NULL || pnode == NULL || link_isset(head,pnode))
		return -1;
	if(head -> next ==NULL)
	{
		head->next = pnode;
		return 0;
	}
	link_node *p;
	for(p = head->next;p->next !=NULL;p = p->next);
	p->next = pnode;
	return 0;
}
int link_insert_head(link_head *head,link_node *pnode)
{
	if(head == NULL || pnode == NULL || link_isset(head,pnode))
		return -1;
	if(head->next != NULL)
		pnode->next = head->next;
	head->next = pnode;
	return 0;
}
int link_isset(link_head *head,link_node *pnode)
{
	if(head ==NULL || pnode ==NULL)
		return -1;
	if(head->next == pnode)
		return 1;
	link_node *p;
	for(p = head->next;p != NULL;p = p->next)
	{
		if(p == pnode)
			return 1;
	}
	return 0;

}

link_node *link_select(link_head *head,const data_t data)
{
	if(head ==NULL || head->next ==NULL)
		return NULL;
	link_node *p;
	for(p = head->next;p != NULL;p = p->next)
	{
		if(strcmp(p->data.str,data.str) == 0)
			return p;
	}
	return NULL;
}
int link_delect(link_head *head,link_node *pnode)
{
	if(head ==NULL || pnode ==NULL)
		return-1;
	link_node *p;
	link_node *pparent;
	for(p=head->next;p!=NULL;p = p->next)
	{
		if(p == pnode)
		{
			pparent->next = p->next;
			free(pnode);
		}
		pparent = p;
	}
	return 0;
}
int link_destroy(link_head *head)
{
	if(head == NULL)
		return -1;
	if(head -> next == NULL)
	{
		free(head);
		return 0;
	}
	link_node *p;
	link_node *pfree;
	for(p=head->next;p!=NULL;p=p->next)
	{
		pfree = p;
		free(p);
		p = pfree;
	}
	free(head);
	return 0;
}

void node_print(link_head *head)
{
	if(head == NULL)
		return;
	link_node *p;
	for(p=head->next;p!=NULL;p=p->next)
	{
		printf("%s\n",p->data.str);
	}
	
}

int recursion_reversal(link_node *prev,link_node *now)
{
	if(prev == NULL || now == NULL)
		return -1;
	if(now->next == NULL){
		now->next = prev;
		return;
	}
	else
	{
		recursion_reversal(now,now->next);
		now->next = prev;
	}
}


int link_reversal(link_head *head)
{
	if(head==NULL || head->next==NULL || head->next->next==NULL)
		return -1;
	link_node *p;
	for(p=head->next;p->next!=NULL;p=p->next);
	recursion_reversal(head->next,head->next->next);
	head->next->next = NULL;
	head->next = p;	
	return 0;
}


int main()
{
	data_t data1 = {"wyw"};
	data_t data2 = {"huangnan"};
	data_t data3 = {"love"};
	link_head *head = head_init();
	link_node *node1 = node_create(data1);
	link_node *node2 = node_create(data2);
	link_node *node3 = node_create(data3);
	link_insert_tail(head,node1);
	link_insert_head(head,node2);
	link_insert(head,node2,node3);
	data_t data4 = {"wyw111"};
	data_t data5 = {"huangnan111"};
	data_t data6 = {"love111"};
	link_node *node4 = node_create(data4);
	link_node *node5 = node_create(data5);
	link_node *node6 = node_create(data6);
	link_insert_tail(head,node4);
	link_insert_head(head,node5);
	link_insert(head,node1,node6);
	
	node_print(head);
	link_reversal(head);

	node_print(head);
	link_destroy(head);
}

