/*************************************************************************
	> File Name: linebuf.h
	> Author: 
	> Mail: 
	> Created Time: Sat 26 Jun 2021 09:21:50 AM PDT
 ************************************************************************/

#ifndef _linebuf_H
#define _linebuf_H

#include <stdio.h>
#include <stdint.h>
typedef struct linebuf
{
	int32_t r_index;
	int32_t w_index;
	int32_t size;
	uint8_t *buf;
}linebuf_t;

void linebuf_init(linebuf_t*l, uint32_t size);
int32_t get_linebuf_len(linebuf_t * l);
int32_t get_linebuf_available_size(linebuf_t * l);
int32_t move_linebuf(linebuf_t * l, int32_t offset);

#endif
