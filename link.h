/*************************************************************************
	> File Name: link.h
	> Author: 
	> Mail: 
	> Created Time: Sat 26 Jun 2021 09:26:30 AM PDT
 ************************************************************************/

#ifndef _LINK_H
#define _LINK_H
#include "linebuf.h"
#include "protocol_hal.h"

typedef struct
{
    LineBuf_t lb;
    protocol_t *p;
    int fd;
} link_t;

#endif
