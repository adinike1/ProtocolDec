/*************************************************************************
	> File Name: link.h
	> Author: 
	> Mail: 
	> Created Time: Sat 26 Jun 2021 09:26:30 AM PDT
 ************************************************************************/

#ifndef _LINK_H
#define _LINK_H
#include <stdio.h>
#include <stdint.h>
#include "linebuf.h"
#include "protocol_hal.h"
#include "list.h"

#define LINK_ERR(fmt, ...)		printf("[LINK] [ERR] "fmt, ##__VA_ARGS__)
#define LINK_INFO(fmt, ...)		printf("[LINK] [INFO] "fmt, ##__VA_ARGS__)

typedef enum
{
    LINK_PRIO_L,
    LINK_PRIO_M,
    LINK_PRIO_H
} link_prio_e;

typedef struct
{
	protocol_t *prot;
	const char *dev_name; /*dev name*/
	int fd; /*dev fd*/
	link_prio_e prio;
	list_t node;
	linebuf_t decbuf;
	int32_t dec_buf_size;
    uint32_t dec_state;
} link_t;

typedef struct
{
	protocol_type_e prot_type;
	const char *dev_name;	/*dev name*/
	int fd;					/*dev fd*/
	link_prio_e prio;
	int32_t dec_buf_size;
} link_cfg_t;

int32_t link_init(void);
#endif
