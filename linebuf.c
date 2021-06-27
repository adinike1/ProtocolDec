/*************************************************************************
	> File Name: linebuf.c
	> Author: 
	> Mail: 
	> Created Time: Sat 26 Jun 2021 09:20:45 AM PDT
 ************************************************************************/

#include <string.h>
#include <stdint.h>
#include "linebuf.h"

void linebuf_init(linebuf_t*l, uint32_t size)
{
    l->buf = malloc(size);
	l->size = size;
}

int32_t get_line_buf_available_size(linebuf_t * l)
{
	return l->size - l->w_index;
}

int32_t get_line_buf_len(linebuf_t * l)
{
	return (l->w_index - l->r_index);
}

int32_t linebuf_write(linebuf_t *l, uint8_t *buf, uint32_t len)
{
    memcpy(l->buf+l->w_index, buf, len);
}
int32_t move_linebuf(linebuf_t * l, int32_t offset)
{
	int32_t buf_size = l->w_index - l->r_index;
	// 移动缓冲区 memcpy 如果与地址重叠，会有什么问题
	while (buf_size > 0)
	{
		buf_size--;
		memcpy(l->buf, l->buf + l->r_index, 1);
	}
	l->r_index = 0;
	l->w_index = buf_size;

	return 0;
}
