/*************************************************************************
	> File Name: my_link.h
	> Author: wyw0907
	> Mail: 827349708@qq.com 
	> Created Time: 2016年09月09日 星期五 11时18分36秒
*************************************************************************/

#ifndef __MY_LINK_H
#define __MY_LINK_H

typedef struct{
	char str[100];
}data_t;

typedef struct link_node{
	data_t data;
	struct link_node*next;
}link_node;

typedef struct{
	link_node *next;
}link_head;

link_head *head_init(void);
link_node *node_create(data_t data);
int link_isset(link_head *,link_node *);
int link_insert(link_head *head,link_node *prevnode,link_node *pnode);
int link_insert_tail(link_head *head,link_node *pnode);
int link_insert_head(link_head *head,link_node *pnode);
link_node *link_select(link_head *head,const data_t data); 
int link_delect(link_head *head,link_node *pnode);
int link_destroy(link_head *head);
void link_print(link_head*);
int link_isset(link_head *,link_node *);
int link_reversal(link_head *);

#endif
