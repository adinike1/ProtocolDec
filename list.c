/*************************************************************************
	> File Name: component/list.c
	> Author: alex.wei
	> Mail: wll_zju@163.com
	> Created Time: Sun 27 Jun 2021 06:02:03 AM PDT
 ************************************************************************/

#include "list.h"

int32_t list_init(list_t *l)
{
    l->prev = l;
    l->next = l;
}

int32_t list_add(list_t *l, list_t *p)
{
    list_t *n = l->prev;
    n->next = p;
    p->next = l;
    p->prev = n;
    l->prev = p;
}

int32_t list_del(list_t *h, list_t *p)
{
    list_t *pre = p->prev;
    list_t *nxt = p->next;
    pre->next = nxt;
    nxt->prev = pre;
}


