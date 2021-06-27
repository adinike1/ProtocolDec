/*************************************************************************
	> File Name: list.h
	> Author: 
	> Mail: 
	> Created Time: Sun 27 Jun 2021 06:26:12 AM PDT
 ************************************************************************/

#ifndef _LIST_H
#define _LIST_H
#include<stdio.h>
#include<stdint.h>

typedef struct list
{
    struct list *next;
    struct list *prev;
} list_t;

#define for_each_list(p, head) \
		for(p=head->next; p!=head; p=p->next)

#define container_of(ptr, type, mem) \
		(type *)(( char *)ptr - (char *)&(((type *)0)->mem))

#define list_entry(ptr, type, mem) \
        container_of(ptr, type, mem)

#define for_each_list_entry(n, head, type, mem) \
		for(n=list_entry(head->next, type, mem); &(n->mem) != head; n = list_entry(n->mem.next, type, mem))

int32_t list_init(list_t *l);
int32_t list_add(list_t *l, list_t *p);
int32_t list_del(list_t *h, list_t *p);

#endif
