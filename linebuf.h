/*************************************************************************
	> File Name: linebuf.h
	> Author: 
	> Mail: 
	> Created Time: Sat 26 Jun 2021 09:21:50 AM PDT
 ************************************************************************/

#ifndef _LINEBUF_H
#define _LINEBUF_H

#include <stdio.h>
#include <stdint.h>
typedef struct LineBuf
{
	int32_t r_index;
	int32_t w_index;
	int32_t size;
	uint8_t *buf;
}LineBuf_t;

void LineBufInit(LineBuf_t*l);
int32_t get_line_buf_len(LineBuf_t * l);
int32_t get_line_buf_available_size(LineBuf_t * l);
int32_t Movelinebuf(LineBuf_t * l, int32_t offset);

#endif
